// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_PRIM_MINIMUM_SPANNING_TREE_HPP
#define BOOST_GRAPH_PYTHON_PRIM_MINIMUM_SPANNING_TREE_HPP

#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/python.hpp>
#include <boost/graph/python/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>

namespace boost { namespace graph { namespace python {

using boost::python::object;

template<typename Graph>
void
prim_minimum_spanning_tree
  (const Graph& g, 
   vector_property_map<
     typename graph_traits<Graph>::vertex_descriptor,
     typename property_map<Graph, vertex_index_t>::const_type>* in_predecessor,
   vector_property_map<
     float,
     typename property_map<Graph, vertex_index_t>::const_type>* in_distance,
   vector_property_map<
     float,
     typename property_map<Graph, edge_index_t>::const_type>* in_weight,
   boost::python::object in_visitor,
   typename graph_traits<Graph>::vertex_descriptor s,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef vector_property_map<Vertex, VertexIndexMap> PredecessorMap;
  typedef vector_property_map<float, VertexIndexMap> DistanceMap;
  typedef vector_property_map<default_color_type, VertexIndexMap> ColorMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;
  typedef vector_property_map<float, EdgeIndexMap> WeightMap;

  if (s == graph_traits<Graph>::null_vertex() && num_vertices(g) > 0)
    s = *vertices(g).first;

  PredecessorMap predecessor = 
    in_predecessor? *in_predecessor
    : PredecessorMap(num_vertices(g), get(vertex_index, g));

  DistanceMap distance = 
    in_distance? *in_distance
    : DistanceMap(num_vertices(g), get(vertex_index, g));

  ColorMap color = 
    in_color? *in_color
    : ColorMap(num_vertices(g), get(vertex_index, g));

  WeightMap weight = 
    in_weight? *in_weight
    : WeightMap(num_edges(g), get(edge_index, g));

  // If no weight map was provided, initialize every weight with 1.0
  if (!in_weight) {
    BGL_FORALL_EDGES_T(e, g, Graph)
      put(weight, e, 1.0f);
  }

  if (in_visitor != object()) {
    boost::prim_minimum_spanning_tree
      (g, predecessor,
       boost::visitor(maybe_dijkstra_visitor(in_visitor)).
       distance_map(distance).
       weight_map(weight).
       color_map(color).
       root_vertex(s));
  } else {
    boost::prim_minimum_spanning_tree
      (g, predecessor,
       distance_map(distance).
       weight_map(weight).
       color_map(color).
       root_vertex(s));
  }
}

} } } // end namespace boost::graph::python

#endif // BOOST_GRAPH_PYTHON_PRIM_MINIMUM_SPANNING_TREE_HPP
