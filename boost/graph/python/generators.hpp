// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_GENERATORS_HPP
#define BOOST_GRAPH_PYTHON_GENERATORS_HPP

#include <memory>
#include <boost/python.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/python/graph.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/plod_generator.hpp>
#include <boost/graph/small_world_generator.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph> 
boost::python::object
erdos_renyi_graph(typename graph_traits<Graph>::vertices_size_type n,
                  double prob, bool allow_self_loops = false, int seed = 1)
{
  typedef erdos_renyi_iterator<minstd_rand, Graph> iterator;
  minstd_rand gen(seed);
  return Graph::pyconstruct(iterator(gen, n, prob, allow_self_loops), 
                            iterator(), n);
}

template<typename Graph> 
boost::python::object
plod_graph(typename graph_traits<Graph>::vertices_size_type n,
           double alpha, double beta, bool allow_self_loops = false,
           int seed = 1)
{
  typedef plod_iterator<minstd_rand, Graph> iterator;
  minstd_rand gen(seed);
  return Graph::pyconstruct(iterator(gen, n, alpha, beta, allow_self_loops), 
                            iterator(), n);
}

template<typename Graph> 
boost::python::object
small_world_graph(typename graph_traits<Graph>::vertices_size_type n,
                  typename graph_traits<Graph>::vertices_size_type k,
                  double prob, bool allow_self_loops = false, 
		  bool allow_multiple_edges = true, int seed = 1)
{
  typedef small_world_iterator<minstd_rand, Graph> iterator;
  minstd_rand gen(seed);
  return Graph::pyconstruct(iterator(gen, n, k, prob, 
				     allow_self_loops, allow_multiple_edges), 
                            iterator(), n);
}

} } } // end namespace boost::graph::python

#endif // BOOST_GRAPH_PYTHON_GENERATORS_HPP
