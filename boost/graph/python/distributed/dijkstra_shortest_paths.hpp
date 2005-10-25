// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/python.hpp>
#include <boost/graph/python/dijkstra_shortest_paths.hpp>

namespace boost { namespace graph { namespace python { namespace distributed {

using boost::python::object;

template<typename Graph>
void
dijkstra_shortest_paths
  (const Graph& g, 
   typename graph_traits<Graph>::vertex_descriptor s,
   vector_property_map<
     typename graph_traits<Graph>::vertex_descriptor,
     typename property_map<Graph, vertex_index_t>::const_type>* in_predecessor,
   vector_property_map<
     float,
     typename property_map<Graph, vertex_index_t>::const_type>* in_distance,
   vector_property_map<
     float,
     typename property_map<Graph, edge_index_t>::const_type>& weight,
   boost::python::object in_visitor,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color,
   float lookahead)
{
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef vector_property_map<Vertex, VertexIndexMap> PredecessorMap;
  typedef vector_property_map<float, VertexIndexMap> DistanceMap;
  typedef vector_property_map<default_color_type, VertexIndexMap> ColorMap;

  PredecessorMap predecessor = 
    in_predecessor? *in_predecessor
    : PredecessorMap(num_vertices(g), get(vertex_index, g));

  DistanceMap distance = 
    in_distance? *in_distance
    : DistanceMap(num_vertices(g), get(vertex_index, g));

  ColorMap color = 
    in_color? *in_color
    : ColorMap(num_vertices(g), get(vertex_index, g));

  if (lookahead >= 0.0) {
    if (in_visitor != object()) {
      boost::dijkstra_shortest_paths
        (g, s, boost::visitor(maybe_dijkstra_visitor(in_visitor)).
         predecessor_map(predecessor).
         distance_map(distance).
         weight_map(weight).
         color_map(color).
         lookahead(lookahead));
    } else {
      boost::dijkstra_shortest_paths
        (g, s, 
         predecessor_map(predecessor).
         distance_map(distance).
         weight_map(weight).
         color_map(color).
         lookahead(lookahead));
    }
  } else {
    if (in_visitor != object()) {
      boost::dijkstra_shortest_paths
        (g, s, boost::visitor(maybe_dijkstra_visitor(in_visitor)).
         predecessor_map(predecessor).
         distance_map(distance).
         weight_map(weight).
         color_map(color));
    } else {
      boost::dijkstra_shortest_paths
        (g, s, 
         predecessor_map(predecessor).
         distance_map(distance).
         weight_map(weight).
         color_map(color));
    }
  }
}

} } } } // end namespace boost::graph::python::distributed
