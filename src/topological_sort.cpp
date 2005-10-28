// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/topological_sort.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <boost/graph/python/list_append_iterator.hpp>

namespace boost { namespace graph { namespace python {

using boost::python::list;

template<typename Graph>
list
topological_sort
  (const Graph& g,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef vector_property_map<
            default_color_type, 
            typename property_map<Graph, vertex_index_t>::const_type> ColorMap;

  ColorMap color = 
    in_color? *in_color : ColorMap(num_vertices(g), get(vertex_index, g));

  list result;
  boost::topological_sort(g, list_append_iterator(result), color_map(color));
  return result;
}

void export_topological_sort()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef vector_property_map<                                        \
              default_color_type,                                       \
              property_map<Graph, vertex_index_t>::const_type> ColorMap; \
    def("topological_sort", &topological_sort<Type>,                    \
        (arg("graph"), arg("color_map") = static_cast<ColorMap*>(0)));  \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
