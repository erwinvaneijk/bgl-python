// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/transitive_closure.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <list>
#include <iterator>

namespace boost { namespace graph { namespace python {

template<typename Graph>
Graph*
transitive_closure
  (const Graph& g,
   vector_property_map<
     typename graph_traits<Graph>::vertex_descriptor,
     typename property_map<Graph, vertex_index_t>::const_type>* g_to_tc_map)
{
  std::auto_ptr<Graph> tc(new Graph);
  if (g_to_tc_map) 
    boost::transitive_closure(g, *tc, *g_to_tc_map, get(vertex_index, g));
  else
    boost::transitive_closure(g, *tc);
  return tc.release();
}

void export_transitive_closure()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::manage_new_object;
  using boost::python::return_value_policy;

#define UNDIRECTED_GRAPH(Name,Type)
#define DIRECTED_GRAPH(Name,Type)                                       \
  {                                                                     \
    typedef vector_property_map<                                        \
              graph_traits<Type>::vertex_descriptor,                    \
              property_map<Type, vertex_index_t>::const_type>           \
      OrigToCopyMap;                                                    \
                                                                        \
    def("transitive_closure", &transitive_closure<Type>,                \
        return_value_policy<manage_new_object>(),                       \
        (arg("graph"), arg("orig_to_copy") = static_cast<OrigToCopyMap*>(0))); \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
