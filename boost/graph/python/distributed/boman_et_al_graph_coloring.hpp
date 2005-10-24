// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/distributed/boman_et_al_graph_coloring.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python { namespace distributed {

using boost::python::object;

template<typename Graph>
std::size_t
boman_et_al_graph_coloring
  (const Graph& g, 
   vector_property_map<
     std::size_t, 
     typename property_map<Graph, vertex_index_t>::type>* in_color,
   typename graph_traits<Graph>::vertices_size_type chunk_size = 100)
{
  typedef typename property_map<Graph, vertex_index_t>::type VertexIndexMap;
  typedef vector_property_map<std::size_t, VertexIndexMap> ColorMap;

  ColorMap color = 
    in_color? *in_color
    : ColorMap(num_vertices(g), get(vertex_index, g));

  return boost::graph::distributed::boman_et_al_graph_coloring(g, color, 
                                                               chunk_size);
}

} } } } // end namespace boost::graph::python::distributed
