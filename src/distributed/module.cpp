// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/python.hpp>
#include "exports.hpp"
#include "config.hpp"
#include "graph_types.hpp"

namespace boost { namespace graph { namespace distributed { namespace python {

BOOST_PYTHON_MODULE(_distributed)
{
  export_mpi();
#define UNDIRECTED_GRAPH(Name,Type) export_graph< Type >(#Name, "undirected");
#define DIRECTED_GRAPH(Name,Type) export_graph< Type >(#Name, "directed");
#include "graphs.hpp"
  export_boman_et_al_graph_coloring();
  export_breadth_first_search();
  export_connected_components();
  export_dehne_gotz_min_spanning_tree();
  export_dijkstra_shortest_paths();
  export_dinic_max_flow();
  export_fruchterman_reingold_force_directed_layout();
  export_page_rank();
  export_strong_components();
}

} } } } // end namespace boost::graph::distributed::python
