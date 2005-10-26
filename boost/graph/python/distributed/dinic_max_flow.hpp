// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/distributed/dinic_max_flow.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python { namespace distributed {

using boost::python::object;

template<typename Graph>
int
dinic_max_flow
  (const Graph& g, 
   typename graph_traits<Graph>::vertex_descriptor s,
   typename graph_traits<Graph>::vertex_descriptor t,
   vector_property_map<
     int,
     typename property_map<Graph, edge_index_t>::const_type>& capacity,
   vector_property_map<
     int,
     typename property_map<Graph, edge_index_t>::const_type>* in_flow)
{
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;
  typedef vector_property_map<int, EdgeIndexMap> FlowMap;

  FlowMap flow = in_flow? *in_flow : FlowMap(num_edges(g), get(edge_index, g));

  return boost::graph::distributed::dinic_max_flow(g, s, t, capacity, flow);
}

} } } } // end namespace boost::graph::python::distributed
