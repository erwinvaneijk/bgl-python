// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Ben Martin
//           Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/sloan_ordering.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <vector>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/python/list_append_iterator.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
boost::python::list 
sloan_ordering
  (Graph& g,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type
   >* in_color,
   vector_property_map<
     float, 
     typename property_map<Graph, vertex_index_t>::const_type
   >* in_priority,
   float weight1,
   float weight2)
{
  typedef typename graph_traits<Graph>::degree_size_type degree_size_type;

  typedef typename property_map<Graph, vertex_index_t>::const_type 
    VertexIndexMap;
  typedef vector_property_map<default_color_type, VertexIndexMap> ColorMap;
  typedef vector_property_map<float, VertexIndexMap> PriorityMap;
  typedef vector_property_map<degree_size_type, VertexIndexMap>
    OutDegreeMap;

  // Out-degree map
  OutDegreeMap out_degree_map(num_vertices(g), get(vertex_index, g));
  BGL_FORALL_VERTICES_T(v, g, Graph)
    put(out_degree_map, v, out_degree(v, g));

  ColorMap color = in_color ? *in_color 
                            : ColorMap(num_vertices(g), get(vertex_index, g));

  PriorityMap priority = in_priority ? *in_priority 
                           : PriorityMap(num_vertices(g), get(vertex_index, g));

  boost::python::list result;
  boost::sloan_ordering(g, list_append_iterator(result),
                        color, out_degree_map, priority, weight1, weight2);
  return result;
}

void export_sloan_ordering()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef property_map<Graph, vertex_index_t>::const_type VertexIndexMap; \
    typedef vector_property_map<default_color_type, VertexIndexMap> ColorMap; \
    typedef vector_property_map<float, VertexIndexMap> PriorityMap;     \
                                                                        \
    def("sloan_ordering",                                               \
        &sloan_ordering<Type>,                                          \
        (arg("graph"),                                                  \
         arg("color") = static_cast<ColorMap*>(0),                      \
         arg("priority") = static_cast<PriorityMap*>(0),                \
         arg("weight1") = 1.0,                                          \
         arg("weight2") = 2.0));                                        \
  }
#define DIRECTED_GRAPH(Name,Type)
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
