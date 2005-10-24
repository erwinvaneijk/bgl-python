// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/distributed/dehne_gotz_min_spanning_tree.hpp>
#include <boost/python.hpp>
#include <boost/graph/python/list_append_iterator.hpp>
#include <boost/graph/distributed/vertex_list_adaptor.hpp>

namespace boost { namespace graph { namespace python { namespace distributed {

using boost::python::object;
using boost::python::list;

template<typename Graph>
list
dense_boruvka_minimum_spanning_tree
  (const Graph& g, 
   vector_property_map<
     double, 
     typename property_map<Graph, edge_index_t>::type> weight)
{
  using boost::graph::make_vertex_list_adaptor;

  list result;
  boost::graph::distributed::dense_boruvka_minimum_spanning_tree
    (make_vertex_list_adaptor(g), weight, list_append_iterator(result));
  return result;
}

} } } } // end namespace boost::graph::python::distributed
