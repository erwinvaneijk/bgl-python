// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/parallel/property_map.hpp>
#include <boost/graph/python/distributed/breadth_first_search.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

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
        &boost::graph::python::distributed::breadth_first_search< Type >, \
        (arg("graph"),                                                  \
         arg("root_vertex"),                                            \
         arg("visitor") = object(),                                     \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
  }
#include "graphs.hpp"
}


} } } } // end namespace boost::graph::distributed::python
