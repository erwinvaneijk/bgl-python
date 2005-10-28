// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/python/page_rank.hpp>

namespace boost { namespace graph { namespace python {

void export_page_rank()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)
#define DIRECTED_GRAPH(Name,Type)               \
  def("page_rank",                              \
      &boost::graph::python::page_rank<Type>,   \
      (arg("graph"), arg("rank_map"), arg("done_or_iterations") = 20, \
       arg("damping") = 0.85));
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
