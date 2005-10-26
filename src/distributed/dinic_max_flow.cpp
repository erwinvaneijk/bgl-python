// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/parallel/property_map.hpp>
#include <boost/graph/python/distributed/dinic_max_flow.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

void export_dinic_max_flow()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)
#define DIRECTED_GRAPH(Name,Type)                                       \
  {                                                                     \
    typedef property_map< Type, edge_index_t >::const_type              \
      EdgeIndexMap;                                                     \
    typedef vector_property_map<int, EdgeIndexMap> EdgeFlowMap;         \
    def("dinic_max_flow",                                               \
        &boost::graph::python::distributed::dinic_max_flow<Type>,       \
        (arg("graph"), arg("source"), arg("sink"), arg("capacity_map"), \
         arg("flow_map") = static_cast<EdgeFlowMap*>(0)));              \
  }
#include "graphs.hpp"
}

} } } } // end namespace boost::graph::distributed::python
