// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_EXPORTS_HPP
#define BOOST_GRAPH_PYTHON_EXPORTS_HPP

#define BGL_GRAPH_CLASS_(G) boost::python::class_<G, boost::noncopyable>

namespace boost { namespace graph { namespace python {

template<typename DirectedS> void export_basic_graph(const char* name);
template<typename Graph> void export_property_maps(const char* name);

template<typename Graph>
void export_generators(BGL_GRAPH_CLASS_(Graph)& graph, const char* name);

template<typename Graph>
void export_graphviz(BGL_GRAPH_CLASS_(Graph)& graph, const char* name);

template<typename Graph>
void export_graphml(BGL_GRAPH_CLASS_(Graph)& graph, const char* name);

template<typename Graph>
boost::python::object
add_vertex_property(Graph& g, const std::string& name, const std::string& type);

template<typename Graph>
boost::python::object
add_edge_property(Graph& g, const std::string& name, const std::string& type);

} } } // end namespace boost::graph::python

#endif // BOOST_GRAPH_PYTHON_EXPORTS_HPP
