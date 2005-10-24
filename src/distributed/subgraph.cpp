// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <boost/graph/python/graph.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

using boost::python::class_;

template<typename Graph>
local_subgraph<Graph> get_local_subgraph(Graph& g)
{
  return local_subgraph<Graph>(g);
}

template<typename Graph>
void export_subgraph(class_<Graph, noncopyable>& graph)
{
  using boost::python::class_;
  using boost::python::no_init;

  typedef local_subgraph<Graph> LocalSubgraph;

  class_<LocalSubgraph> local_subgraph("LocalSubgraph", no_init);
  boost::graph::python::vertex_list_graph<LocalSubgraph> vlsg(local_subgraph);
  boost::graph::python::edge_list_graph<LocalSubgraph> elsg(local_subgraph);
  boost::graph::python::incidence_graph<LocalSubgraph> isg(local_subgraph);
  boost::graph::python::bidirectional_graph<LocalSubgraph> bsg(local_subgraph);
  boost::graph::python::adjacency_graph<LocalSubgraph> asg(local_subgraph);    

  // Returning the local subgraph of a graph
  graph.def("local_subgraph", &get_local_subgraph<Graph>);  
}

template void export_subgraph<Graph>(class_<Graph, noncopyable>&);

} } } } // end namespace boost::graph::distributed::python
