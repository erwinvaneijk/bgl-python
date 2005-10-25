// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/parallel/property_map.hpp>
#include <boost/graph/python/distributed/dehne_gotz_min_spanning_tree.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

void export_dehne_gotz_min_spanning_tree()
{
  using boost::python::arg;
  using boost::python::def;

  using boost::graph::python::distributed::dense_boruvka_minimum_spanning_tree;
  using boost::graph::python::distributed::merge_local_minimum_spanning_trees;
  using boost::graph::python::distributed::boruvka_then_merge;
  using boost::graph::python::distributed::boruvka_mixed_merge;

#define UNDIRECTED_GRAPH(Name,Type)                     \
  def("dense_boruvka_minimum_spanning_tree",            \
      &dense_boruvka_minimum_spanning_tree<Type>,       \
      (arg("graph"), arg("weight_map")));               \
                                                        \
  def("merge_local_minimum_spanning_trees",             \
      &merge_local_minimum_spanning_trees<Type>,        \
      (arg("graph"), arg("weight_map")));               \
                                                        \
  def("boruvka_then_merge",                             \
      &boruvka_then_merge<Type>,                        \
      (arg("graph"), arg("weight_map")));               \
                                                        \
  def("boruvka_mixed_merge",                            \
      &boruvka_mixed_merge<Type>,                       \
      (arg("graph"), arg("weight_map")));
#define DIRECTED_GRAPH(Name,Type)
#include "graphs.hpp"
}

} } } } // end namespace boost::graph::distributed::python
