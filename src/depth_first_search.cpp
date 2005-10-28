// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/undirected_dfs.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

#define BGL_PYTHON_VISITOR maybe_dfs_visitor
#define BGL_PYTHON_EVENTS_HEADER <boost/graph/python/dfs_events.hpp>
#include <boost/graph/python/visitor.hpp>
#undef BGL_PYTHON_EVENTS_HEADER
#undef BGL_PYTHON_VISITOR

using boost::python::object;

template<typename Graph>
void 
depth_first_search
  (const Graph& g,
   typename graph_traits<Graph>::vertex_descriptor s,
   object in_visitor,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef vector_property_map<
            default_color_type, 
            typename property_map<Graph, vertex_index_t>::const_type>
    ColorMap;

  ColorMap color = 
    in_color? *in_color : ColorMap(num_vertices(g), get(vertex_index, g));

  if (s == graph_traits<Graph>::null_vertex() && num_vertices(g) > 0)
    s = *vertices(g).first;

  if (in_visitor != object()) {
    boost::depth_first_search(g, maybe_dfs_visitor(in_visitor), color, s);
  } else {
    boost::depth_first_search(g, boost::dfs_visitor<>(), color, s);
  }
}
 
template<typename Graph>
void 
depth_first_visit
  (const Graph& g,
   typename graph_traits<Graph>::vertex_descriptor s,
   object in_visitor,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color)
{
  typedef vector_property_map<
            default_color_type, 
            typename property_map<Graph, vertex_index_t>::const_type>
    ColorMap;

  ColorMap color = 
    in_color? *in_color : ColorMap(num_vertices(g), get(vertex_index, g));

  if (s == graph_traits<Graph>::null_vertex() && num_vertices(g) > 0)
    s = *vertices(g).first;

  if (in_visitor != object()) {
    boost::depth_first_visit(g, s, maybe_dfs_visitor(in_visitor), color);
  } else {
    boost::depth_first_visit(g, s, boost::dfs_visitor<>(), color);
  }
}

template<typename Graph>
void 
undirected_dfs
  (const Graph& g,
   object in_visitor,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, vertex_index_t>::const_type>* in_color,
   vector_property_map<
     default_color_type, 
     typename property_map<Graph, edge_index_t>::const_type>* in_edge_color)
{
  typedef vector_property_map<
            default_color_type, 
            typename property_map<Graph, vertex_index_t>::const_type>
    ColorMap;

  ColorMap color = 
    in_color? *in_color : ColorMap(num_vertices(g), get(vertex_index, g));

  typedef vector_property_map<
            default_color_type, 
            typename property_map<Graph, edge_index_t>::const_type>
    EdgeColorMap;

  EdgeColorMap edge_color = 
    in_edge_color? *in_edge_color 
    : EdgeColorMap(num_edges(g), get(edge_index, g));  

  if (in_visitor != object()) {
    boost::undirected_dfs(g, maybe_dfs_visitor(in_visitor), color, edge_color);
  } else {
    boost::undirected_dfs(g, boost::dfs_visitor<>(), color, edge_color);
  }
}

void export_depth_first_search() 
{ 
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  DIRECTED_GRAPH(Name,Type)                                             \
  {                                                                     \
    typedef vector_property_map<                                        \
              default_color_type,                                       \
              property_map<Type, vertex_index_t>::const_type> ColorMap; \
    typedef vector_property_map<                                        \
              default_color_type,                                       \
              property_map<Type, edge_index_t>::const_type> EdgeColorMap; \
                                                                        \
    def("undirected_dfs", &undirected_dfs<Type>,                        \
        (arg("graph"),                                                  \
         arg("visitor") = object(),                                     \
         arg("color_map") = static_cast<ColorMap*>(0),                  \
         arg("edge_color_map") = static_cast<EdgeColorMap*>(0)));       \
  }

#define DIRECTED_GRAPH(Name,Type)                                       \
  {                                                                     \
    typedef vector_property_map<                                        \
              default_color_type,                                       \
              property_map<Type, vertex_index_t>::const_type> ColorMap; \
                                                                        \
    def("depth_first_search", &depth_first_search<Type>,                \
        (arg("graph"),                                                  \
         arg("root_vertex") = graph_traits<Type>::null_vertex(),        \
         arg("visitor") = object(),                                     \
         arg("color_map") = static_cast<ColorMap*>(0)));                \
                                                                        \
    def("depth_first_visit", &depth_first_visit<Type>,                  \
        (arg("graph"),                                                  \
         arg("root_vertex"),                                            \
         arg("visitor") = object(),                                     \
         arg("color_map") = static_cast<ColorMap*>(0)));                \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
