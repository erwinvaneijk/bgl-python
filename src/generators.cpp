// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/python/generators.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
void export_generators(boost::python::class_<Graph>& graph)
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::manage_new_object;
  using boost::python::return_value_policy;

  // Generators
  graph
    .def("erdos_renyi_graph",
         &boost::graph::python::erdos_renyi_graph<Graph>,
         return_value_policy<manage_new_object>(),
         (arg("num_vertices"), arg("probability"),
          arg("allow_self_loops") = false, arg("random_seed") = 1))
    .staticmethod("erdos_renyi_graph")
    .def("plod_graph",
         &boost::graph::python::plod_graph<Graph>,
         return_value_policy<manage_new_object>(),
         (arg("num_vertices"), arg("alpha"), arg("beta"),
          arg("allow_self_loops") = false, arg("random_seed") = 1))
    .staticmethod("plod_graph")
    .def("small_world_graph",
         &boost::graph::python::small_world_graph<Graph>,
         return_value_policy<manage_new_object>(),
         (arg("num_vertices"), arg("num_neighbors"), arg("rewire_probability"),
          arg("allow_self_loops") = false, arg("random_seed") = 1))
    .staticmethod("small_world_graph");
}

#define UNDIRECTED_GRAPH(Name,Type) \
  template void export_generators<Type>(boost::python::class_< Type >& graph);
#include "graphs.hpp"

} } } // end namespace boost::graph::python
