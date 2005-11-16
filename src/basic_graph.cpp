// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "basic_graph.hpp"
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/python/graph.hpp>
#include "exports.hpp"
#include "graph_types.hpp"
#include <boost/algorithm/string/replace.hpp>

namespace boost { 

inline std::ostream& operator<<(std::ostream& out, default_color_type c)
{
  switch (c) {
  case white_color: return out << "white";
  case gray_color: return out << "gray";
  case green_color: return out << "green";
  case red_color: return out << "red";
  case black_color: return out << "black";
  }
  return out;
}

inline std::istream& operator>>(std::istream& in, default_color_type& c)
{
  std::string text;
  if (in >> text) {
    if (text == "white") c = white_color;
    else if (text == "gray") c = gray_color;
    else if (text == "green") c = green_color;
    else if (text == "red") c = red_color;
    else if (text == "black") c = black_color;
    else {
      in.setstate(std::ios_base::failbit);
      return in;
    }
  }
  return in;
}

namespace graph { namespace python {

template<typename DirectedS>
struct build_string_property_maps
{
  build_string_property_maps(basic_graph<DirectedS>* g) : g(g) { }

  std::auto_ptr<boost::dynamic_property_map>
  operator()(const std::string& name, const boost::any& key, 
             const boost::any& value)
  {
    typedef typename basic_graph<DirectedS>::VertexIndexMap VertexIndexMap;
    typedef typename basic_graph<DirectedS>::EdgeIndexMap   EdgeIndexMap;

    std::auto_ptr<boost::dynamic_property_map> result(0);

    if (key.type() == typeid(typename basic_graph<DirectedS>::Vertex)) {
      typedef vector_property_map<std::string, VertexIndexMap>
        property_map_type;
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset
        (new adaptor_type(property_map_type(num_vertices(*g),
                                            get(vertex_index, *g))));
    } else if (key.type() == typeid(typename basic_graph<DirectedS>::Edge)) {
      typedef vector_property_map<std::string, EdgeIndexMap> property_map_type;
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset
        (new adaptor_type(property_map_type(num_edges(*g),
                                            get(edge_index, *g))));
    } 
    return result;
  }

private:
  basic_graph<DirectedS>* g;
};

// ----------------------------------------------------------
// Constructors
// ----------------------------------------------------------
#ifdef BOOST_MSVC
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif
template<typename DirectedS>
basic_graph<DirectedS>::basic_graph() 
  : inherited(), dp(build_string_property_maps<DirectedS>(this))
{ }

template<typename DirectedS>
basic_graph<DirectedS>::basic_graph(boost::python::object l,
                                    const std::string& name_map)
  : inherited(), dp(build_string_property_maps<DirectedS>(this))
{
  using boost::python::object;
  std::map<object, vertex_descriptor> verts;
  int len = ::boost::python::extract<int>(l.attr("__len__")());

  vector_property_map<object, VertexIndexMap> name;

  for (int  i = 0; i < len; ++i) {
    vertex_descriptor u, v;
    object up = l[i][0];
    object vp = l[i][1];

    typename std::map<object, vertex_descriptor>::iterator pos;
    pos = verts.find(up);
    if (pos == verts.end()) {
      u = verts[up] = add_vertex();
      if (!name_map.empty()) name[u] = up;
    }
    else u = pos->second;
    pos = verts.find(vp);
    if (pos == verts.end()) {
      v = verts[vp] = add_vertex();
      if (!name_map.empty()) name[v] = vp;
    }
    else v = pos->second;

    add_edge(u, v);
  }

  if (!name_map.empty())
    dp.property(name_map, name);
}
#ifdef BOOST_MSVC
#  pragma warning (pop)
#endif

// ----------------------------------------------------------
// Mutable graph
// ----------------------------------------------------------
template<typename DirectedS>
typename basic_graph<DirectedS>::Vertex basic_graph<DirectedS>::add_vertex()
{
  using boost::add_vertex;

  base_vertex_descriptor v = add_vertex(base());
  put(vertex_index, base(), v, index_to_vertex.size());
  index_to_vertex.push_back(v);
  return Vertex(v);
}

template<typename DirectedS>
void basic_graph<DirectedS>::clear_vertex(Vertex vertex)
{
  // Remove all incoming and outgoing edges
  while (out_degree(vertex, *this) > 0) remove_edge(*out_edges(vertex, *this).first);
  while (in_degree(vertex, *this) > 0) remove_edge(*in_edges(vertex, *this).first);
}

template<typename DirectedS>
void basic_graph<DirectedS>::remove_vertex(Vertex vertex)
{
  using boost::remove_vertex;

  // Update property maps
  for (dynamic_properties::iterator i = dp.begin(); i != dp.end(); ++i) {
    if (i->second->key() == typeid(Vertex)) {
      dynamic_cast<python_dynamic_property_map*>(&*i->second)->
        copy_value(vertex, Vertex(index_to_vertex.back()));
    }
  }

  // Update vertex indices
  std::size_t index = get(vertex_index, base(), vertex.base);
  index_to_vertex[index] = index_to_vertex.back();
  put(vertex_index, base(), index_to_vertex[index], index);
  index_to_vertex.pop_back();

  // Remove the actual vertex
  remove_vertex(vertex.base, base());
}

template<typename DirectedS>
typename basic_graph<DirectedS>::Edge 
basic_graph<DirectedS>::add_edge(Vertex u, Vertex v)
{
  using boost::add_edge;

  base_edge_descriptor e = add_edge(u.base, v.base, base()).first;
  put(edge_index, base(), e, index_to_edge.size());
  index_to_edge.push_back(e);
  return Edge(e);
}

template<typename DirectedS>
void basic_graph<DirectedS>::remove_edge(Edge edge)
{
  using boost::remove_edge;

  // Update property maps
  for (dynamic_properties::iterator i = dp.begin(); i != dp.end(); ++i) {
    if (i->second->key() == typeid(Edge)) {
      dynamic_cast<python_dynamic_property_map*>(&*i->second)->
        copy_value(edge, Edge(index_to_edge.back()));
    }
  }

  // Update edge indices
  std::size_t index = get(edge_index, base(), edge.base);
  index_to_edge[index] = index_to_edge.back();
  put(edge_index, base(), index_to_edge[index], index);
  index_to_edge.pop_back();

  // Remove the actual edge
  remove_edge(edge.base, base());
}

template<typename DirectedS>
void basic_graph<DirectedS>::renumber_vertices()
{
  using boost::vertices;

  BGL_FORALL_VERTICES_T(v, base(), inherited) {
    put(vertex_index, base(), v, index_to_vertex.size());
    index_to_vertex.push_back(v);
  }
}

template<typename DirectedS>
void basic_graph<DirectedS>::renumber_edges()
{
  using boost::edges;

  BGL_FORALL_EDGES_T(e, base(), inherited) {
    put(edge_index, base(), e, index_to_edge.size());
    index_to_edge.push_back(e);
  }
}

template<typename Graph> void export_in_graph();

template<typename DirectedS>
void export_basic_graph(const char* name)
{
  using boost::python::arg;
  using boost::python::class_;
  using boost::python::init;
  using boost::python::object;
  using boost::python::range;
  using boost::python::scope;
  using boost::python::self;
  using boost::python::tuple;

  typedef basic_graph<DirectedS> Graph;
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor Edge;
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;

  {
    scope s;

    class_<Graph> graph(name);

    std::string my_graph_init_doc(graph_init_doc);
    algorithm::replace_all(my_graph_init_doc, "GRAPH", std::string(name));

    graph
        // Constructors
        .def(init<object>())
        .def(init<object, std::string>(my_graph_init_doc.c_str()))
        .def("is_directed", &Graph::is_directed,
             "is_directed(self) -> bool\n\nWhether the graph is directed or not.")
        // Pickling
        .def_pickle(graph_pickle_suite<DirectedS>())
      ;

    boost::graph::python::graph<Graph> g(graph);
    boost::graph::python::vertex_list_graph<Graph> vlg(graph);
    boost::graph::python::edge_list_graph<Graph> elg(graph);
    boost::graph::python::incidence_graph<Graph> ig(graph);
    boost::graph::python::bidirectional_graph<Graph> bg(graph);
    boost::graph::python::adjacency_graph<Graph> ag(graph);
    boost::graph::python::mutable_graph<Graph, false, false> mg(graph);

    export_generators(graph, name);
    export_graphviz(graph, name);

    // Properties
    export_property_maps<Graph>();
    graph.def("vertex_property_map", &vertex_property_map<Graph>,
              (arg("graph"), arg("type")),
              vertex_property_map_doc);
    graph.def("edge_property_map", &edge_property_map<Graph>,
              (arg("graph"), arg("type")),
              edge_property_map_doc);
  }
}

void export_graphs()
{
  export_basic_graph<undirectedS>("Graph");
  export_basic_graph<bidirectionalS>("Digraph");
}

using boost::python::object;

template void basic_graph<undirectedS>::renumber_vertices();
template void basic_graph<undirectedS>::renumber_edges();
template void basic_graph<bidirectionalS>::renumber_vertices();
template void basic_graph<bidirectionalS>::renumber_edges();

} } } // end namespace boost::graph::python
