// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include "exports.hpp"
#include <boost/graph/python/generators.hpp>
#include <boost/python.hpp>
#include <string>
#include <boost/algorithm/string/replace.hpp>

namespace boost { namespace graph { namespace python {

const char* erdos_renyi_doc = 
"erdos_renyi_graph(num_vertices, probability, allow_self_loops = False,\n"
"                  random_seed = 1) -> GRAPH\n\n"
"Constructs a new Erdos-Renyi random graph with num_vertices vertices and\n"
"a uniform probability of having an edge (u, v) in the graph for any\n"
"vertices u and v. Expect a graph with probability*num_vertices^2 edges.\n\n"
"Parameters:\n"
"  num_vertices      The number of vertices in the graph. \n"
"  probability       The probability of having an edge (u, v).\n"
"  allow_self_loops  Whether self-loops (u, u) will be generated.\n"
"  random_seed       Nonzero seed value for the random number generator.\n\n"
"Complete C++ documentation is available at:\n"
"  http://www.boost.org/libs/graph/doc/erdos_renyi_generator.html\n"

  ;

const char* plod_doc = 
"plod_graph(num_vertices, alpha, beta, allow_self_loops = False, \n"
"           random_seed = 1) -> GRAPH\n\n"
"Constructs a new power law graph with num_vertices vertices. The number\n"
"of connections to a given vertex is beta*x^(-alpha), where alpha and\n"
"beta are parameters to the algorithm and x is a random variable between\n"
"0 and num_vertices - 1.\n\n"
"Parameters:\n"
"  num_vertices      The number of vertices in the graph.\n"
"  alpha             The exponential fall-off in the degree distribution.\n"
"  beta              Controls how many edges will occur in the graph.\n"
"  allow_self_loops  Whether self-loops (u, u) will be generated.\n"
"  random_seed       Nonzero seed value for the random number generator.\n\n"
"Complete C++ documentation is available at:\n"
"  http://www.boost.org/libs/graph/doc/plod_generator.html\n"
  ;

const char* small_world_doc = 
"small_world_graph(num_vertices, num_neighbors, rewire_probability,\n"
"           allow_self_loops = False, random_seed = 1) -> GRAPH\n\n"
"Constructs a new small-world graph with num_vertices vertices, each\n"
"adjacent to its num_neighbors closest neighbors (assume that the \n"
"vertcices were arranged in a circle). With probability\n"
"rewire_probability, an edge will be rewired randomly.\n\n"
"Parameters:\n"
"  num_vertices          The number of vertices in the graph.\n"
"  num_neighbors         The number of neighbors each vertex starts with.\n"
"  rewire_probability    Probability of rewiring any given edge.\n"
"  allow_self_loops      Whether self-loops (u, u) will be generated.\n"
"  allow_multiple_edges  Whether multiple edges between vertices are generated.\n"
"  random_seed         Nonzero seed for the random number generator.\n\n"
"Complete C++ documentation is available at:\n"
"  http://www.boost.org/libs/graph/doc/small_world_generator.html\n"
  ;

template<typename Graph>
void export_generators(BGL_GRAPH_CLASS_(Graph)& graph, const char* name)
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::manage_new_object;
  using boost::python::return_value_policy;

  std::string my_erdos_renyi_doc(erdos_renyi_doc);
  algorithm::replace_all(my_erdos_renyi_doc, std::string("GRAPH"), 
                         std::string(name));
  std::string my_plod_doc(plod_doc);
  algorithm::replace_all(my_plod_doc, std::string("GRAPH"), std::string(name));
  std::string my_small_world_doc(small_world_doc);
  algorithm::replace_all(my_small_world_doc, 
                         std::string("GRAPH"), std::string(name));

  // Generators
  graph
    .def("erdos_renyi_graph",
         &boost::graph::python::erdos_renyi_graph<Graph>,
         (arg("num_vertices"), arg("probability"),
          arg("allow_self_loops") = false, arg("random_seed") = 1),
         my_erdos_renyi_doc.c_str())
    .staticmethod("erdos_renyi_graph")
    .def("plod_graph",
         &boost::graph::python::plod_graph<Graph>,
         (arg("num_vertices"), arg("alpha"), arg("beta"),
          arg("allow_self_loops") = false, arg("random_seed") = 1),
         my_plod_doc.c_str())
    .staticmethod("plod_graph")
    .def("small_world_graph",
         &boost::graph::python::small_world_graph<Graph>,
         (arg("num_vertices"), arg("num_neighbors"), arg("rewire_probability"),
          arg("allow_self_loops") = false, arg("allow_multiple_edges") = true, 
	  arg("random_seed") = 1),
         my_small_world_doc.c_str())
    .staticmethod("small_world_graph");
}

#define UNDIRECTED_GRAPH(Name,Type) \
  template void export_generators<Type>(BGL_GRAPH_CLASS_(Type)&, \
                                        const char*);
#include "graphs.hpp"

} } } // end namespace boost::graph::python
