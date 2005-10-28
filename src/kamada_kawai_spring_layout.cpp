// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <boost/graph/circle_layout.hpp>
#include "graph_types.hpp"
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

using boost::python::object;

template<typename Graph>
void 
kamada_kawai_spring_layout
  (const Graph& g,
   vector_property_map<
     point2d, 
     typename property_map<Graph, vertex_index_t>::const_type>& position,
   vector_property_map<
     float, 
     typename property_map<Graph, edge_index_t>::const_type>* in_weight,
   float side_length,
   boost::python::object done,
   float spring_constant,
   bool progressive)
{
  typedef vector_property_map<
            float, 
            typename property_map<Graph, edge_index_t>::const_type> WeightMap;

  WeightMap weight = 
    in_weight? *in_weight : WeightMap(num_edges(g), get(edge_index, g));

  // If we weren't give a weight map, assume unweighted edges
  if (!in_weight) BGL_FORALL_EDGES_T(e, g, Graph) put(weight, e, 1.0f);

  if (!progressive) circle_graph_layout(g, position, side_length/2);
  
  if (done != object()) {
    boost::kamada_kawai_spring_layout(g, position, weight, 
                                      boost::side_length(side_length), done, 
                                      spring_constant);
  } else {
    boost::kamada_kawai_spring_layout(g, position, weight, 
                                      boost::side_length(side_length), 
                                      layout_tolerance<float>(),
                                      spring_constant);
  }
}

void export_kamada_kawai_spring_layout()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef vector_property_map<                                        \
              float,                                                    \
              property_map<Graph, edge_index_t>::const_type> WeightMap; \
                                                                        \
    def("kamada_kawai_spring_layout",                                   \
        &kamada_kawai_spring_layout<Type>,                              \
        (arg("graph"),                                                  \
         arg("position"),                                               \
         arg("weight") = static_cast<WeightMap*>(0),                    \
         arg("side_length") = 500.0,                                    \
         arg("done") = object(),                                        \
         arg("spring_constant") = 1.0,                                  \
         arg("progressive") = false));                                  \
  }
#define DIRECTED_GRAPH(Name,Type)
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
