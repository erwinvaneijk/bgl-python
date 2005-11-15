// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/bc_clustering.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
void 
brandes_betweenness_centrality
  (const Graph& g, 
   vector_property_map<
     float,
     typename property_map<Graph, vertex_index_t>::const_type
   >* in_vertex_centrality,
   vector_property_map<
     float, 
     typename property_map<Graph, edge_index_t>::const_type
   >* in_edge_centrality,
   vector_property_map<
     float, 
     typename property_map<Graph, edge_index_t>::const_type>* weight)
{
  typedef vector_property_map<
            float, typename property_map<Graph, vertex_index_t>::const_type> 
    VertexCentralityMap;

  typedef vector_property_map<
            float, typename property_map<Graph, edge_index_t>::const_type> 
    EdgeCentralityMap;

  VertexCentralityMap vertex_centrality = 
    in_vertex_centrality? *in_vertex_centrality 
    : VertexCentralityMap(num_vertices(g), get(vertex_index, g));

  EdgeCentralityMap edge_centrality = 
    in_edge_centrality? *in_edge_centrality 
    : EdgeCentralityMap(num_edges(g), get(edge_index, g));

  if (weight) {
    boost::brandes_betweenness_centrality
      (g, 
       weight_map(*weight).
       centrality_map(vertex_centrality).
       edge_centrality_map(edge_centrality));
  } else {
    boost::brandes_betweenness_centrality
      (g, 
       centrality_map(vertex_centrality).
       edge_centrality_map(edge_centrality));
  }
}

template<typename Graph>
void 
relative_betweenness_centrality
  (const Graph& g, 
   vector_property_map<
     float, 
     typename property_map<Graph, vertex_index_t>::const_type>& centrality)
{ 
  relative_betweenness_centrality(g, centrality); 
}

template<typename Graph>
float
central_point_dominance
  (const Graph& g, 
   vector_property_map<
     float, 
     typename property_map<Graph, vertex_index_t>::const_type>& centrality)
{ 
  return boost::central_point_dominance(g, centrality); 
}

struct bc_clustering_done_python
{
  explicit bc_clustering_done_python(boost::python::object done) 
    : done(done) { }

  template<typename Graph>
  bool 
  operator()(float max_centrality, 
             typename graph_traits<Graph>::edge_descriptor e,
             const Graph& g)
  {
    using boost::python::extract;
    return extract<bool>(done(max_centrality, e, ref(g)));
  }

private:
  boost::python::object done;
};

template<typename Graph>
void 
betweenness_centrality_clustering
  (Graph& g, boost::python::object done,
   vector_property_map<
     float, 
     typename property_map<Graph, edge_index_t>::const_type
   >* in_edge_centrality)
{
  typedef vector_property_map<
            float, typename property_map<Graph, edge_index_t>::const_type> 
    EdgeCentralityMap;

  EdgeCentralityMap edge_centrality = 
    in_edge_centrality? *in_edge_centrality 
    : EdgeCentralityMap(num_edges(g), get(edge_index, g));

  boost::betweenness_centrality_clustering(g, 
                                           bc_clustering_done_python(done),
                                           edge_centrality);
}

void export_betweenness_centrality()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef property_map<Type, vertex_index_t>::const_type VertexIndexMap;  \
    typedef property_map<Type, edge_index_t>::const_type EdgeIndexMap;      \
    typedef vector_property_map<float, VertexIndexMap> VertexCentralityMap; \
    typedef vector_property_map<float, EdgeIndexMap> EdgeCentralityMap; \
                                                                        \
    def("brandes_betweenness_centrality",                               \
        &brandes_betweenness_centrality<Type>,                          \
        (arg("graph"),                                                  \
         arg("vertex_centrality_map") = static_cast<VertexCentralityMap*>(0), \
         arg("edge_centrality_map") = static_cast<EdgeCentralityMap*>(0), \
         arg("weight_map") = static_cast<EdgeCentralityMap*>(0)));      \
    def("relative_betweenness_centrality",                              \
        &relative_betweenness_centrality<Type>,                         \
        (arg("graph"), arg("vertex_centrality_map")));                  \
    def("central_point_dominance",                                      \
        &central_point_dominance<Type>,                                 \
        (arg("graph"), arg("vertex_centrality_map")));                  \
    def("betweenness_centrality_clustering",                            \
        &betweenness_centrality_clustering<Type>,                       \
        (arg("graph"), arg("done"),                                     \
         arg("edge_centrality_map") = static_cast<EdgeCentralityMap*>(0))); \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
