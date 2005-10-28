// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/python/connected_components.hpp>

namespace boost { namespace graph { namespace python {

void export_connected_components()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef property_map<Type, vertex_index_t>::const_type VertexIndexMap; \
    typedef vector_property_map<int, VertexIndexMap>                    \
      VertexColorMap;                                                   \
                                                                        \
    def("connected_components",                                         \
        &boost::graph::python::connected_components<Type>,              \
        (arg("graph"),                                                  \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
  }
#define DIRECTED_GRAPH(Name,Type)
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
