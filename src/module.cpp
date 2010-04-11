// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/python.hpp>
#include "exports.hpp"
#include "graph_types.hpp"

namespace boost { namespace graph { namespace python {

extern void export_exceptions();
extern void export_graphs();
extern void export_astar_search();
extern void export_bellman_ford_shortest_paths();
extern void export_breadth_first_search();
extern void export_connected_components();
extern void export_dijkstra_shortest_paths();
extern void export_fruchterman_reingold_force_directed_layout();
extern void export_betweenness_centrality();
extern void export_biconnected_components();
extern void export_circle_graph_layout();
extern void export_cuthill_mckee_ordering();
extern void export_dag_shortest_paths();
extern void export_depth_first_search();
extern void export_isomorphism();
extern void export_kamada_kawai_spring_layout();
extern void export_king_ordering();
extern void export_kruskal_minimum_spanning_tree();
extern void export_minimum_degree_ordering();
extern void export_page_rank();
extern void export_prim_minimum_spanning_tree();
extern void export_sequential_vertex_coloring();
extern void export_sloan_ordering();
#ifdef INCLUDE_SPECTRUM
extern void export_spectral_embedding_layout();
extern void export_spectral_embedding_3d_layout();
extern void export_spectrum();
//#else
//#warning "Not building spectrum() and related functions."
#endif
#ifdef INCLUDE_SPARSE_SPECTRUM
extern void export_sparse_spectrum();
#endif
extern void export_strong_components();
extern void export_topological_sort();
extern void export_transitive_closure();
extern void export_graphviz_exceptions();

BOOST_PYTHON_MODULE(_graph)
{
  export_exceptions();
  export_graphviz_exceptions();
  export_graphs();
  export_astar_search();
  export_bellman_ford_shortest_paths();
  export_betweenness_centrality();
  export_biconnected_components();
  export_breadth_first_search();
  export_circle_graph_layout();
  export_connected_components();
  export_cuthill_mckee_ordering();
  export_dag_shortest_paths();
  export_depth_first_search();
  export_dijkstra_shortest_paths();
  export_fruchterman_reingold_force_directed_layout();
  export_isomorphism();
  export_kamada_kawai_spring_layout();
  export_king_ordering();
  export_kruskal_minimum_spanning_tree();
  export_minimum_degree_ordering();
  export_page_rank();
  export_prim_minimum_spanning_tree();
  export_sequential_vertex_coloring();
  export_sloan_ordering();
#ifdef INCLUDE_SPECTRUM
  export_spectral_embedding_layout();
  export_spectral_embedding_3d_layout();
  export_spectrum();
//#else
//#warning "Not using spectrum()."
#endif
#ifdef INCLUDE_SPARSE_SPECTRUM
  export_sparse_spectrum();
#endif
  export_strong_components();
  export_topological_sort();
  export_transitive_closure();
}

} } } // end namespace boost::graph::python
