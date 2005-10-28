// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/sequential_vertex_coloring.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
int
sequential_vertex_coloring
  (const Graph& g, 
   vector_property_map<
     int, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef vector_property_map<
            int, typename property_map<Graph, vertex_index_t>::const_type>
    ColorMap;

  ColorMap color = 
    in_color? *in_color 
    : ColorMap(num_vertices(g), get(vertex_index, g));

  return boost::sequential_vertex_coloring(g, color);
}

void export_sequential_vertex_coloring()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef vector_property_map<                                        \
              int, property_map<Type, vertex_index_t>::const_type> ColorMap; \
                                                                        \
    def("sequential_vertex_coloring",                                   \
        &sequential_vertex_coloring<Type>,                             \
        (arg("graph"), arg("color_map") = static_cast<ColorMap*>(0)));  \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
