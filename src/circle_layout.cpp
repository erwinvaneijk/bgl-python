// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/circle_layout.hpp>
#include "graph_types.hpp"
#include <boost/graph/python/point2d.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
void 
circle_graph_layout
  (const Graph& g,
   vector_property_map<
     point2d, 
     typename property_map<Graph, vertex_index_t>::const_type>& position,
   float radius)
{
  boost::circle_graph_layout(g, position, radius);
}

void export_circle_graph_layout()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  def("circle_graph_layout",                                            \
      &circle_graph_layout<Type>,                                       \
      (arg("graph"), arg("position_map"), arg("radius") = 0.5));
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
