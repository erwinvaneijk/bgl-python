// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"

namespace boost {
  // Work around an annoying qualified "boost::get" call.
  using boost::graph::python::get;
}

#include <boost/graph/astar_search.hpp>
#include <boost/graph/iteration_macros.hpp>

namespace boost { namespace graph { namespace python {

#define BGL_PYTHON_VISITOR maybe_astar_visitor
#define BGL_PYTHON_EVENTS_HEADER <boost/graph/python/astar_events.hpp>
#include <boost/graph/python/visitor.hpp>
#undef BGL_PYTHON_EVENTS_HEADER
#undef BGL_PYTHON_VISITOR

using boost::python::object;

// Function object adaptor for the A* heuristic
template<typename Graph>
struct astar_heuristic_adaptor
{
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

  astar_heuristic_adaptor(object h) : h(h) { }

  float operator()(Vertex v) 
  { 
    return boost::python::extract<float>(h(v));
  }

 private:
  object h;
};

template<typename Graph>
void 
astar_search
  (const Graph& g,
   typename graph_traits<Graph>::vertex_descriptor s,
   object in_heuristic,
   object in_visitor,
   vector_property_map<
      typename graph_traits<Graph>::vertex_descriptor,
      typename property_map<Graph, vertex_index_t>::const_type>* in_predecessor,
   vector_property_map<
      float,
      typename property_map<Graph, vertex_index_t>::const_type>* in_cost,
   vector_property_map<
      float,
      typename property_map<Graph, vertex_index_t>::const_type>* in_distance,
   vector_property_map<
      float,
      typename property_map<Graph, edge_index_t>::const_type>* in_weight,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;
  typedef vector_property_map<vertex_descriptor, VertexIndexMap> PredecessorMap;
  typedef vector_property_map<float, VertexIndexMap> CostMap;
  typedef vector_property_map<float, VertexIndexMap> DistanceMap;
  typedef vector_property_map<float, EdgeIndexMap> WeightMap;
  typedef vector_property_map<default_color_type, VertexIndexMap> ColorMap;

  PredecessorMap predecessor(in_predecessor? *in_predecessor
                             /*default*/: PredecessorMap(num_vertices(g), 
                                                         get(vertex_index, g)));
  CostMap cost(in_cost? *in_cost
               /*default*/: CostMap(num_vertices(g), get(vertex_index, g)));
  DistanceMap distance(in_distance? *in_distance
                       /*default*/: DistanceMap(num_vertices(g), 
                                                get(vertex_index, g)));
  WeightMap weight(in_weight? *in_weight
                   /*default*/: WeightMap(num_edges(g), get(edge_index, g)));
  ColorMap color(in_color? *in_color 
                 /*default*/: ColorMap(num_vertices(g), get(vertex_index, g)));

  if (!in_weight)
    BGL_FORALL_EDGES_T(e, g, Graph)
      put(weight, e, 1.0);

  if (in_visitor != object()) {
    typedef queue<vertex_descriptor> Queue;
    maybe_astar_visitor vis(in_visitor);
    boost::astar_search(g, s, astar_heuristic_adaptor<Graph>(in_heuristic),
                        predecessor_map(predecessor).
                        rank_map(cost).
                        distance_map(distance).
                        weight_map(weight).
                        color_map(color).
                        visitor(vis));
  } else {
    boost::astar_search(g, s, astar_heuristic_adaptor<Graph>(in_heuristic),
                        predecessor_map(predecessor).
                        rank_map(cost).
                        distance_map(distance).
                        weight_map(weight).
                        color_map(color));
  }
}

void export_astar_search()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef property_map< Type , vertex_index_t>::const_type            \
      VertexIndexMap;                                                   \
    typedef property_map< Type , edge_index_t>::const_type              \
      EdgeIndexMap;                                                     \
    typedef graph_traits< Type >::vertex_descriptor vertex_descriptor;  \
    typedef vector_property_map<vertex_descriptor, VertexIndexMap>      \
      PredecessorMap;                                                   \
    typedef vector_property_map<float, VertexIndexMap> CostMap;         \
    typedef vector_property_map<float, VertexIndexMap> DistanceMap;     \
    typedef vector_property_map<float, EdgeIndexMap> WeightMap;         \
    typedef vector_property_map<default_color_type, VertexIndexMap>     \
      VertexColorMap;                                                   \
                                                                        \
    def("astar_search",                                                 \
        &boost::graph::python::astar_search< Type >,                    \
        (arg("graph"),                                                  \
         arg("root_vertex"),                                            \
         arg("heuristic"),                                              \
         arg("visitor") = object(),                                     \
         arg("predecessor_map") = static_cast<PredecessorMap*>(0),      \
         arg("cost_map") = static_cast<CostMap*>(0),                    \
         arg("distance_map") = static_cast<DistanceMap*>(0),            \
         arg("weight_map") = static_cast<WeightMap*>(0),                \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
  }
#include "graphs.hpp"
}


} } } // end namespace boost::graph::python
