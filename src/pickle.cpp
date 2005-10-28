// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/iteration_macros.hpp>

namespace boost { namespace graph { namespace python {

template<typename DirectedS>
boost::python::tuple 
graph_pickle_suite<DirectedS>::getstate(boost::python::object g_obj)
{
  using boost::python::tuple;
  using boost::python::make_tuple;
  using boost::python::list;
  using boost::python::extract;
  using boost::python::dict;
  using boost::python::object;

  const Graph& g = extract<const Graph&>(g_obj)();
  typename property_map<Graph, vertex_index_t>::const_type 
    vertex_index_map = get(vertex_index, g);
  typename property_map<Graph, edge_index_t>::const_type 
    edge_index_map = get(edge_index, g);

  dict vertex_properties;
  dict edge_properties;

  // Collect edges
  std::vector<tuple> the_edges(num_edges(g));
  BGL_FORALL_EDGES_T(e, g, Graph)
    the_edges[get(edge_index_map, e)] = 
      make_tuple(get(vertex_index_map, source(e, g)),
                 get(vertex_index_map, target(e, g)));

  list edges_list;
  for (std::vector<tuple>::iterator i = the_edges.begin(); 
       i != the_edges.end(); ++i)
    edges_list.append(*i);

  // Collect vertex and edge properties
  const dynamic_properties& dp = g.get_dynamic_properties();
  for (dynamic_properties::const_iterator pm = dp.begin(); 
       pm != dp.end(); ++pm) {
    python_dynamic_property_map* pmap = 
      dynamic_cast<python_dynamic_property_map*>(pm->second);
    if (pm->second->key() == typeid(Vertex)) {
      std::vector<object> values(num_vertices(g));
      BGL_FORALL_VERTICES_T(v, g, Graph)
        values[get(vertex_index_map, v)] = pmap->get_python(v);

      list values_list;
      for (std::vector<object>::iterator i = values.begin(); 
           i != values.end(); ++i)
        values_list.append(*i);
      vertex_properties[pm->first] = tuple(values_list);
    } else if (pm->second->key() == typeid(Edge)) {
      std::vector<object> values(num_edges(g));
      BGL_FORALL_EDGES_T(e, g, Graph)
        values[get(edge_index_map, e)] = pmap->get_python(e);

      list values_list;
      for (std::vector<object>::iterator i = values.begin(); 
           i != values.end(); ++i)
        values_list.append(*i);
      edge_properties[pm->first] = tuple(values_list);
    } else {
      assert(false);
    }
  }

  return make_tuple(g_obj.attr("__dict__"), 
                    num_vertices(g),
                    edges_list,
                    vertex_properties,
                    edge_properties);
}

template<typename DirectedS>
void
graph_pickle_suite<DirectedS>::setstate(boost::python::object g_obj, 
                                        boost::python::tuple state)
{
  using boost::python::tuple;
  using boost::python::make_tuple;
  using boost::python::list;
  using boost::python::extract;
  using boost::python::dict;
  using boost::python::object;

  Graph& g = extract<Graph&>(g_obj)();

  // restore the graph's __dict__
  dict d = extract<dict>(g_obj.attr("__dict__"))();
  d.update(state[0]);

  // Get the number of vertices
  typedef typename graph_traits<Graph>::vertices_size_type vertices_size_type;
  vertices_size_type n = extract<vertices_size_type>(state[1]);
  std::vector<Vertex> vertices;
  vertices.reserve(n);
  while (vertices.size() < n) vertices.push_back(g.add_vertex());

  // Get the edges
  typedef typename graph_traits<Graph>::edges_size_type edges_size_type;
  std::vector<Edge> the_edges;
  list edges_list = extract<list>(state[2]);
  edges_size_type m = extract<int>(edges_list.attr("__len__")());
  the_edges.reserve(m);
  for (unsigned i = 0; i < m; ++i) {
    tuple e = extract<tuple>(edges_list[i]);
    the_edges.push_back
      (g.add_edge(vertices[extract<vertices_size_type>(e[0])],
                  vertices[extract<vertices_size_type>(e[1])]));
  }
  
  // Get the vertex properties
  typedef typename Graph::VertexIndexMap VertexIndexMap;
  dict vertex_properties = extract<dict>(state[3]);
  list vertex_map_names = vertex_properties.keys();
  while (vertex_map_names != list()) {
    object name_obj = vertex_map_names.pop(0);
    const char* name = extract<const char*>(name_obj);
    vector_property_map<object, VertexIndexMap> pmap = 
      g.get_vertex_object_map(name);
    tuple values = extract<tuple>(vertex_properties[name_obj]);
    for (vertices_size_type i = 0; i < num_vertices(g); ++i)
      put(pmap, vertices[i], values[i]);
  }

  // Get the edge properties
  typedef typename Graph::EdgeIndexMap EdgeIndexMap;
  dict edge_properties = extract<dict>(state[4]);
  list edge_map_names = edge_properties.keys();
  while (edge_map_names != list()) {
    object name_obj = edge_map_names.pop(0);
    const char* name = extract<const char*>(name_obj);
    vector_property_map<object, EdgeIndexMap> pmap = 
      g.get_edge_object_map(name);
    tuple values = extract<tuple>(edge_properties[name_obj]);
    for (edges_size_type i = 0; i < num_edges(g); ++i)
      put(pmap, the_edges[i], values[i]);
  }
}

template struct graph_pickle_suite<undirectedS>;
template struct graph_pickle_suite<bidirectionalS>;

} } } // end namespace boost::graph::python

