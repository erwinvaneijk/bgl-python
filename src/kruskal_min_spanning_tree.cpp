// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include "graph_types.hpp"
#include <boost/graph/python/list_append_iterator.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
boost::python::list 
kruskal_minimum_spanning_tree
  (const Graph& g,
   vector_property_map<
     float, 
     typename property_map<Graph, edge_index_t>::const_type>& weight)
 {
  boost::python::list result;
  boost::kruskal_minimum_spanning_tree(g, list_append_iterator(result),
                                       weight_map(weight)
                                       .vertex_index_map(get(vertex_index, g)));
  return result;
}

void export_kruskal_minimum_spanning_tree()
{
  using boost::python::arg;
  using boost::python::def;

  def("kruskal_minimum_spanning_tree", &kruskal_minimum_spanning_tree<Graph>,
      (arg("graph"), arg("weight_map")));
}

} } } // end namespace boost::graph::python
