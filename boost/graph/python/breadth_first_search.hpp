// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/python/queue.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

using boost::python::object;

#define BGL_PYTHON_VISITOR maybe_bfs_visitor
#define BGL_PYTHON_EVENTS_HEADER <boost/graph/python/bfs_events.hpp>
#include <boost/graph/python/visitor.hpp>
#undef BGL_PYTHON_EVENTS_HEADER
#undef BGL_PYTHON_VISITOR

template<typename Graph>
void 
breadth_first_search
  (const Graph& g,
   typename graph_traits<Graph>::vertex_descriptor s,
   object in_Q,
   object in_visitor,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;

  typedef vector_property_map<
            default_color_type, 
            typename property_map<Graph, vertex_index_t>::const_type>
    ColorMap;

  ColorMap color(in_color? *in_color 
                 /*default*/: ColorMap(num_vertices(g), get(vertex_index, g)));

  bool has_default_queue = in_Q == object();
  if (!has_default_queue || in_visitor != object()) {
    typedef queue<vertex_descriptor> Queue;
    maybe_python_queue<Queue> Q(in_Q, Queue());
    maybe_bfs_visitor vis(in_visitor);
    boost::breadth_first_search(g, s, color_map(color).visitor(vis).buffer(Q));
  } else {
    boost::breadth_first_search(g, s, color_map(color));
  }
}

} } } // end namespace boost::graph::python
