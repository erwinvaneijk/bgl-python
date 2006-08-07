// Copyright (C)2005, 2006 The Trustees of Indiana University.

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

namespace boost { namespace graph { namespace python {

template<typename DirectedS>
boost::python::object basic_graph<DirectedS>::graph_type;

template<typename DirectedS>
boost::python::handle<> basic_graph<DirectedS>::py_null_vertex;

// ----------------------------------------------------------
// Constructors
// ----------------------------------------------------------
#ifdef BOOST_MSVC
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif
template<typename DirectedS>
basic_graph<DirectedS>::basic_graph(PyObject* self) 
  : inherited(), 
    vertex_objects(0, get(vertex_index, *this)),
    edge_objects(0, get(edge_index, *this)),
    self(self)
{ 
  initialize();
}

template<typename DirectedS>
basic_graph<DirectedS>::basic_graph(PyObject* self, boost::python::object l,
                                    const std::string& name_map)
  : inherited(), 
    vertex_objects(0, get(vertex_index, *this)),
    edge_objects(0, get(edge_index, *this)),
    self(self)
{
  initialize();

  using boost::python::object;
  std::map<object, vertex_descriptor> verts;
  int len = ::boost::python::extract<int>(l.attr("__len__")());

  typedef vector_property_map<object, VertexIndexMap> name_map_type;
  typedef resizable_vector_property_map<object, VertexIndexMap> 
    resize_name_map_type;
  name_map_type name(1, get(vertex_index, *this));

  if (!name_map.empty()) {
    std::auto_ptr<resizable_property_map> reg(new resize_name_map_type(name));
    register_vertex_map(reg);
    vertex_properties()[name_map] = object(name);
  }

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
}
#ifdef BOOST_MSVC
#  pragma warning (pop)
#endif

template<typename DirectedS>
basic_graph<DirectedS>::~basic_graph() 
{
  for (std::list<resizable_property_map*>::iterator v = vertex_maps.begin();
       v != vertex_maps.end(); ++v)
    delete *v;
  for (std::list<resizable_property_map*>::iterator e = edge_maps.begin();
       e != edge_maps.end(); ++e)
    delete *e;
}

// ----------------------------------------------------------
// Mutable graph
// ----------------------------------------------------------
template<typename DirectedS>
typename basic_graph<DirectedS>::Vertex basic_graph<DirectedS>::add_vertex()
{
  using boost::add_vertex;
  using boost::python::handle;

  // Add the vertex and give it an index
  base_vertex_descriptor v = add_vertex(base());
  put(vertex_index, base(), v, index_to_vertex.size());
  index_to_vertex.push_back(Vertex(v, this));

  // Update any vertex property maps around
  for (std::list<resizable_property_map*>::iterator i = vertex_maps.begin();
       i != vertex_maps.end(); /* in loop */) {
    if ((*i)->added_key()) ++i;
    else {
      delete *i;
      i = vertex_maps.erase(i);
    }
  }

  // Create a Python object for this vertex and put it into the
  // vertex_objects property map.
  using boost::python::objects::value_holder;
  using boost::python::objects::make_instance;
  using boost::python::objects::class_cref_wrapper;
  typedef class_cref_wrapper<Vertex, 
                             make_instance<Vertex, value_holder<Vertex> > >
    Converter;

  Vertex result(v, this);
  boost::python::object vertex_object(handle<>(Converter::convert(result)));
  put(vertex_objects, result, vertex_object);

  return result;
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

  // Update any vertex property maps around
  for (std::list<resizable_property_map*>::iterator i = vertex_maps.begin();
       i != vertex_maps.end(); /* in loop */) {
    if ((*i)->removed_key(get(vertex_index, base(), vertex.base))) ++i;
    else {
      delete *i;
      i = vertex_maps.erase(i);
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
  using boost::python::handle;

  // Add the edge
  base_edge_descriptor e = add_edge(u.base, v.base, base()).first;
  put(edge_index, base(), e, index_to_edge.size());
  index_to_edge.push_back(Edge(e, this));

  // Update any edge property maps around
  for (std::list<resizable_property_map*>::iterator i = edge_maps.begin();
       i != edge_maps.end(); /* in loop */) {
    if ((*i)->added_key()) ++i;
    else {
      delete *i;
      i = edge_maps.erase(i);
    }
  }

  // Create a Python object for this edge and put it into the
  // edge_objects property map.
  using boost::python::objects::value_holder;
  using boost::python::objects::make_instance;
  using boost::python::objects::class_cref_wrapper;
  typedef class_cref_wrapper<Edge, 
                             make_instance<Edge, value_holder<Edge> > >
    Converter;

  Edge result(e, this);
  boost::python::object edge_object(handle<>(Converter::convert(result)));
  put(edge_objects, result, edge_object);

  return result;
}

template<typename DirectedS>
void basic_graph<DirectedS>::remove_edge(Edge edge)
{
  using boost::remove_edge;

  // Update any edge property maps around
  for (std::list<resizable_property_map*>::iterator i = edge_maps.begin();
       i != edge_maps.end(); /* in loop */) {
    if ((*i)->removed_key(get(edge_index, base(), edge.base))) ++i;
    else {
      delete *i;
      i = edge_maps.erase(i);
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
std::pair<typename basic_graph<DirectedS>::Edge, bool> 
basic_graph<DirectedS>::edge(Vertex u, Vertex v) const
{
  using boost::edge;
  std::pair<base_edge_descriptor, bool> result = edge(u, v, base());
  return std::make_pair(Edge(result.first, this), result.second);
}

template<typename DirectedS>
void basic_graph<DirectedS>::initialize()
{
  // Register the cached Python objects for each vertex and edge.
  typedef resizable_vector_property_map<boost::python::object,
                                        VertexIndexMap> vertex_object_map;
  typedef resizable_vector_property_map<boost::python::object,
                                        EdgeIndexMap> edge_object_map;

  std::auto_ptr<resizable_property_map> 
    reg_vertex_objects(new vertex_object_map(vertex_objects));
  register_vertex_map(reg_vertex_objects);

  std::auto_ptr<resizable_property_map> 
    reg_edge_objects(new edge_object_map(edge_objects));
  register_edge_map(reg_edge_objects);
}

template<typename DirectedS>
void basic_graph<DirectedS>::renumber_vertices()
{
  using boost::vertices;

  BGL_FORALL_VERTICES_T(v, base(), inherited) {
    put(vertex_index, base(), v, index_to_vertex.size());
    index_to_vertex.push_back(vertex_descriptor(v, this));
  }
}

template<typename DirectedS>
void basic_graph<DirectedS>::renumber_edges()
{
  using boost::edges;

  BGL_FORALL_EDGES_T(e, base(), inherited) {
    put(edge_index, base(), e, index_to_edge.size());
    index_to_edge.push_back(edge_descriptor(e, this));
  }
}

template<typename Graph>
boost::python::object 
py_edge(const Graph& g, 
        typename graph_traits<Graph>::vertex_descriptor u,
        typename graph_traits<Graph>::vertex_descriptor v)
{
  std::pair<typename graph_traits<Graph>::edge_descriptor, bool> 
    result = edge(u, v, g);
  if (result.second) return boost::python::object(result.first);
  else return boost::python::object();
}

template<typename Graph>
boost::python::dict py_get_vertex_properties(const Graph& g)
{
  return g.vertex_properties();
}

template<typename Graph>
boost::python::dict py_get_edge_properties(const Graph& g)
{
  return g.edge_properties();
}

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
/* The following routines are needed to work around a Visual C++ 7.1 bug
   where add_property() can't tell the difference between a docstring
   and a setter for a read-only property. So, we give it setters that
   throw Python exceptions. */

template<typename Graph>
void py_set_vertex_properties(const Graph&, boost::python::dict)
{
  PyErr_SetString(PyExc_AttributeError, 
                  "object attribute 'vertex_properties' is read-only");
}

template<typename Graph>
void py_set_edge_properties(const Graph&, boost::python::dict)
{
  PyErr_SetString(PyExc_AttributeError, 
                  "object attribute 'edge_properties' is read-only");
}
#endif

template<typename Graph> void export_in_graph();

static const char* vertex_properties_doc = 
      "A Python dictionary mapping from vertex property names to\n"
       "property maps. These properties are \"attached\" to the graph\n"
       "and will be pickled or serialized along with the graph.";
static const char* edge_properties_doc =
       "A Python dictionary mapping from edge property names to\n"
       "property maps. These properties are \"attached\" to the graph\n"
       "and will be pickled or serialized along with the graph.";

// Defined and instantiated in convert_properties.cpp
template<typename Graph>
void export_property_map_conversions(BGL_GRAPH_CLASS_(Graph)& graph);

/* Function object that retrieves the cached Python object
   corresponding to the given vertex value. */
template<typename Graph>
struct cached_vertex_object {
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  static PyObject* convert(const Vertex& v) {
    if (v == graph_traits<Graph>::null_vertex()) {
      Py_INCREF(Graph::py_null_vertex.get());
      return Graph::py_null_vertex.get();
    }
    else {
      PyObject* result = get(v.graph->vertex_objects, v).ptr();
      Py_INCREF(result);
      return result;
    }
  }
};

/* Function object that retrieves the cached Python object
   corresponding to the given edge value. */
template<typename Edge>
struct cached_edge_object {
  static PyObject* convert(const Edge& e) {
    PyObject* result = get(e.graph->edge_objects, e).ptr();
    Py_INCREF(result);
    return result;
  }
};

template<typename DirectedS>
void export_basic_graph(const char* name)
{
  using boost::python::arg;
  using boost::python::class_;
  using boost::python::dict;
  using boost::python::init;
  using boost::python::object;
  using boost::python::range;
  using boost::python::scope;
  using boost::python::self;
  using boost::python::tuple;
  using boost::python::to_python_converter;

  typedef basic_graph<DirectedS> Graph;
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor Edge;
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;

  {
    scope s;

    class_<Graph, boost::noncopyable> graph(name);
    Graph::graph_type = graph;
    
    // Build documentation string
    std::string my_graph_init_doc(graph_init_doc);
    algorithm::replace_all(my_graph_init_doc, "GRAPH", std::string(name));

    graph
        // Constructors
        .def(init<object, std::string>((arg("edges"), 
                                        arg("name_map") = "node_id"),
                                       my_graph_init_doc.c_str()))
        .def("is_directed", &Graph::is_directed,
             "is_directed(self) -> bool\n\nWhether the graph is directed or not.")
        // Properties
        .add_property("vertex_properties", &py_get_vertex_properties<Graph>,
#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
                      &py_set_vertex_properties<Graph>,
#endif 
                      vertex_properties_doc
                      )
        .add_property("edge_properties", &py_get_edge_properties<Graph>,
#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
                      &py_set_edge_properties<Graph>,
#endif
                      edge_properties_doc
                      )
        // Miscellaneous adjacency list functions
        .def("edge", &py_edge<Graph>,
             (arg("graph"), arg("u"), arg("v")),
             "edge(self, u, v) -> Edge\n\n"
             "Returns an edge (u, v) if one exists, otherwise None.")
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

    // Create a Python object for this vertex and put it into the
    // vertex_objects property map.
    using boost::python::objects::value_holder;
    using boost::python::objects::make_instance;
    using boost::python::objects::class_cref_wrapper;
    using boost::python::handle;
    using boost::python::borrowed;

    typedef class_cref_wrapper<Vertex, 
                               make_instance<Vertex, value_holder<Vertex> > >
      Converter;
    
    Graph::py_null_vertex = handle<>(Converter::convert(Graph::null_vertex()));

    // Register conversion routines that access the cached vertex/edge
    // objects in the graph.
    to_python_converter<Vertex, cached_vertex_object<Graph> >();
    to_python_converter<Edge, cached_edge_object<Edge> >();

    // Other miscellaneous routines

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
    export_property_map_conversions<Graph>(graph);
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
