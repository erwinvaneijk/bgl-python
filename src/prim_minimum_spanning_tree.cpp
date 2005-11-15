// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/python/prim_minimum_spanning_tree.hpp>

namespace boost { namespace graph { namespace python {

void export_prim_minimum_spanning_tree()
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
    typedef vector_property_map<default_color_type, VertexIndexMap>     \
      VertexColorMap;                                                   \
    typedef vector_property_map<float, EdgeIndexMap>                    \
      EdgeWeightMap;                                                    \
                                                                        \
    def("prim_minimum_spanning_tree",                                      \
        &boost::graph::python::prim_minimum_spanning_tree<Type>, \
        (arg("graph"),                                                  \
         arg("predecessor_map") = static_cast<VertexPredecessorMap*>(0), \
         arg("distance_map") = static_cast<VertexDistanceMap*>(0),      \
         arg("weight_map") = static_cast<EdgeWeightMap*>(0),            \
         arg("visitor") = object(),                                     \
         arg("root_vertex") = graph_traits<Type>::null_vertex(),        \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
  }
#include "graphs.hpp"
}


} } } // end namespace boost::graph::python
