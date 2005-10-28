// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/python/breadth_first_search.hpp>

namespace boost { namespace graph { namespace python {

void export_breadth_first_search()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef property_map< Type , vertex_index_t>::const_type VertexIndexMap; \
    typedef vector_property_map<default_color_type, VertexIndexMap>     \
      VertexColorMap;                                                   \
                                                                        \
    def("breadth_first_search",                                         \
        &boost::graph::python::breadth_first_search< Type >,            \
        (arg("graph"),                                                  \
         arg("root_vertex"),                                            \
         arg("buffer") = object(),                                      \
         arg("visitor") = object(),                                     \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
  }
#include "graphs.hpp"
}


} } } // end namespace boost::graph::python
