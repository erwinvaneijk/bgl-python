// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/python/strong_components.hpp>

namespace boost { namespace graph { namespace python {

void export_strong_components()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)
#define DIRECTED_GRAPH(Name,Type)                                       \
  {                                                                     \
    typedef property_map<Type, vertex_index_t>::const_type VertexIndexMap; \
    typedef vector_property_map<int, VertexIndexMap>                    \
      VertexColorMap;                                                   \
                                                                        \
    def("strong_components",                                            \
        &boost::graph::python::strong_components<Type>,                 \
        (arg("graph"),                                                  \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
