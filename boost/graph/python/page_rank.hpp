// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/page_rank.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python { 

using boost::python::object;

struct python_done
{
  explicit python_done(object callable) : callable(callable) { }

  template<typename RankMap, typename Graph>
  bool operator()(RankMap rank, const Graph& g)
  {
    using boost::python::extract;
    return extract<bool>(callable(rank, ref(g)));
  }

private:
  object callable;
};

template<typename Graph>
void
page_rank
  (const Graph& g, 
   vector_property_map<
     float, 
     typename property_map<Graph, vertex_index_t>::const_type>& rank,
   object done_or_iterations,
   float damping)
{
  using boost::python::extract;

  int iterations = 20;
  if (done_or_iterations != object()) {
    extract<int> iters(done_or_iterations);
    if (iters.check())
      iterations = iters;
    else {
      boost::graph::page_rank(g, rank, python_done(done_or_iterations), 
                              damping);
      return;
    } 
  }

  boost::graph::page_rank(g, rank, boost::graph::n_iterations(iterations),
                          damping);
}

} } } // end namespace boost::graph::python
