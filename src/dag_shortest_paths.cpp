// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/dag_shortest_paths.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <boost/graph/python/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>

namespace boost { namespace graph { namespace python {

using boost::python::object;

template<typename Graph>
void
dag_shortest_paths
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
     typename property_map<Graph, edge_index_t>::const_type>* in_weight,
   object in_visitor)
{
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef vector_property_map<Vertex, VertexIndexMap> PredecessorMap;
  typedef vector_property_map<float, VertexIndexMap> DistanceMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;
  typedef vector_property_map<float, EdgeIndexMap> WeightMap;

  PredecessorMap predecessor = 
    in_predecessor? *in_predecessor
    : PredecessorMap(num_vertices(g), get(vertex_index, g));

  DistanceMap distance = 
    in_distance? *in_distance
    : DistanceMap(num_vertices(g), get(vertex_index, g));

  WeightMap weight = 
    in_weight? *in_weight
    : WeightMap(num_edges(g), get(edge_index, g));

  // If no weight map was provided, initialize every weight with 1.0
  if (!in_weight) {
    BGL_FORALL_EDGES_T(e, g, Graph)
      put(weight, e, 1.0f);
  }

  if (in_visitor != object()) { 
    boost::dag_shortest_paths
      (g, s, 
       visitor(maybe_dijkstra_visitor(in_visitor)).
       predecessor_map(predecessor).
       distance_map(distance).
       weight_map(weight));
  } else {
    boost::dag_shortest_paths
      (g, s, 
       predecessor_map(predecessor).
       distance_map(distance).
       weight_map(weight));
  }
}

void export_dag_shortest_paths()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef graph_traits<Type>::vertex_descriptor vertex_descriptor;    \
    typedef property_map<Type, vertex_index_t>::const_type VertexIndexMap; \
    typedef property_map<Type, edge_index_t>::const_type EdgeIndexMap;  \
    typedef vector_property_map<vertex_descriptor, VertexIndexMap>      \
      VertexPredecessorMap;                                             \
    typedef vector_property_map<float, VertexIndexMap>                  \
      VertexDistanceMap;                                                \
    typedef vector_property_map<float, EdgeIndexMap>                    \
      EdgeWeightMap;                                                    \
                                                                        \
    def("dag_shortest_paths",                                           \
        &dag_shortest_paths<Type>,                                      \
        (arg("graph"),                                                  \
         arg("root_vertex"),                                            \
         arg("predecessor_map") = static_cast<VertexPredecessorMap*>(0), \
         arg("distance_map") = static_cast<VertexDistanceMap*>(0),      \
         arg("weight_map") = static_cast<EdgeWeightMap*>(0),            \
         arg("visitor") = object()));                                   \
  }
#include "graphs.hpp"
}


} } } // end namespace boost::graph::python
