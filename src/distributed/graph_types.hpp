// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_PARALLEL_GRAPH_PYTHON_GRAPH_TYPES_HPP
#define BOOST_PARALLEL_GRAPH_PYTHON_GRAPH_TYPES_HPP

#include "config.hpp"
#include <boost/graph/distributed/adjacency_list.hpp>

namespace boost { namespace graph { namespace distributed {namespace python {

typedef adjacency_list<vecS, 
                       distributedS<process_group_type, vecS>,
                       undirectedS,
                       no_property,
                       property<edge_index_t, std::size_t> >
  Graph; 

typedef adjacency_list<vecS, 
                       distributedS<process_group_type, vecS>,
                       bidirectionalS,
                       no_property,
                       property<edge_index_t, std::size_t> >
  Digraph; 

} } } } // end namespace boost::graph::distributed::python

#endif // BOOST_PARALLEL_GRAPH_PYTHON_GRAPH_TYPES_HPP
