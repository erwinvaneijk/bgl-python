// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/graph/python/breadth_first_search.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
void 
breadth_first_visit
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
    boost::breadth_first_visit(g, s, color_map(color).visitor(vis).buffer(Q));
  } else {
    boost::breadth_first_visit(g, s, color_map(color));
  }
}

void export_breadth_first_search()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef property_map< Type , vertex_index_t>::const_type VertexIndexMap; \
    typedef vector_property_map<default_color_type, VertexIndexMap>     \
      VertexColorMap;                                                   \
                                                                        \
    def("breadth_first_search",                                         \
        &boost::graph::python::breadth_first_search< Type >,            \
        (arg("graph"),                                                  \
         arg("root_vertex"),                                            \
         arg("buffer") = object(),                                      \
         arg("visitor") = object(),                                     \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
    def("breadth_first_visit",                                          \
        &boost::graph::python::breadth_first_visit< Type >,             \
        (arg("graph"),                                                  \
         arg("root_vertex"),                                            \
         arg("buffer") = object(),                                      \
         arg("visitor") = object(),                                     \
         arg("color_map") = static_cast<VertexColorMap*>(0)));          \
  }
#include "graphs.hpp"
}


} } } // end namespace boost::graph::python
