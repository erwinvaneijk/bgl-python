// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/python/breadth_first_search.hpp>

namespace boost { namespace graph { namespace python { namespace distributed {

using boost::python::object;

template<typename Graph>
void 
breadth_first_search
  (const Graph& g,
   typename graph_traits<Graph>::vertex_descriptor s,
   object in_visitor,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;

  typedef vector_property_map
            <default_color_type, 
             typename property_map<Graph, vertex_index_t>::const_type>
    ColorMap;

  ColorMap color(in_color? *in_color 
                 /*default*/: ColorMap(num_vertices(g), get(vertex_index, g)));

  if (in_visitor != object()) {
    maybe_bfs_visitor vis(in_visitor);
    boost::breadth_first_search(g, s, color_map(color).visitor(vis));
  } else {
    boost::breadth_first_search(g, s, color_map(color));
  }
}

} } } } // end namespace boost::graph::python::distributed
