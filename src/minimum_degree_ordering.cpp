// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Ben Martin
//           Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/minimum_degree_ordering.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <vector>
#include <boost/graph/iteration_macros.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
boost::python::list 
minimum_degree_ordering
(Graph& g,
 vector_property_map<
   int, typename property_map<Graph, vertex_index_t>::const_type
 >* in_supernode_size,
 int delta)
{
  typedef std::vector<int> InversePermutationMap;
  typedef std::vector<int> PermutationMap;
  typedef typename property_map<Graph, vertex_index_t>::const_type 
    VertexIndexMap;
  typedef vector_property_map<int, VertexIndexMap> OutDegreeMap;
  typedef vector_property_map<int, VertexIndexMap> SuperNodeSizeMap;

  typedef typename graph_traits<Graph>::vertices_size_type vertices_size_type;

  vertices_size_type n = num_vertices(g);

  // Out-degree map
  OutDegreeMap out_degree_map(num_vertices(g), get(vertex_index, g));
  BGL_FORALL_VERTICES_T(v, g, Graph)
    put(out_degree_map, v, out_degree(v, g));

  InversePermutationMap inv_perm_map(n, 0);
  PermutationMap perm_map(n, 0);
  SuperNodeSizeMap supernode_size = 
    in_supernode_size ? *in_supernode_size 
    : SuperNodeSizeMap(num_vertices(g), get(vertex_index, g));
  BGL_FORALL_VERTICES_T(v, g, Graph)
    supernode_size[v] = 1;

  boost::minimum_degree_ordering(g, out_degree_map, inv_perm_map, perm_map,
				 supernode_size, delta,
                                 get(vertex_index, g));

  // Build the new ordering
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  std::vector<vertex_descriptor> new_ordering(num_vertices(g));
  BGL_FORALL_VERTICES_T(v, g, Graph)
    new_ordering[perm_map[get(vertex_index, g, v)]] = v;

  // Build resulting list for Python
  boost::python::list result;
  typedef typename std::vector<vertex_descriptor>::iterator iterator;
  for (iterator i = new_ordering.begin(); i != new_ordering.end(); ++i)
    result.append(*i);

  return result;
}

void export_minimum_degree_ordering()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)
#define DIRECTED_GRAPH(Name,Type)                                       \
  {                                                                     \
    typedef vector_property_map<int,                                    \
                                property_map<Type, vertex_index_t>::const_type>\
      SuperNodeSizeMap;                                                 \
                                                                        \
    def("minimum_degree_ordering",                                      \
        &minimum_degree_ordering<Type>,                                 \
        (arg("graph"),                                                  \
         arg("supernode_size") = static_cast<SuperNodeSizeMap*>(0),     \
         arg("delta") = 0));                                            \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
