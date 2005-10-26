// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_DISTRIBUTED_EXPORTS_HPP
#define BOOST_GRAPH_PYTHON_DISTRIBUTED_EXPORTS_HPP

#include <string>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

void export_mpi();

template<typename Graph> 
void export_graph(const char* name, const char* directedness);

void export_boman_et_al_graph_coloring();
void export_breadth_first_search();
void export_connected_components();
void export_dijkstra_shortest_paths();
void export_dinic_max_flow();
void export_dehne_gotz_min_spanning_tree();
void export_fruchterman_reingold_force_directed_layout();
void export_page_rank();
void export_strong_components();

template<typename Graph>
boost::python::object 
vertex_property_map(const Graph& g, const std::string type);

template<typename Graph>
boost::python::object 
edge_property_map(const Graph& g, const std::string type);

template<typename Graph> void export_property_maps();

template<typename Graph> void export_generators(const std::string& directness);

template<typename Graph> void export_metis(const std::string& directedness);

template<typename Graph> void export_graphviz(const std::string& directedness);

} } } } // end namespace boost::graph::distributed::python

#endif // BOOST_GRAPH_PYTHON_DISTRIBUTED_EXPORTS_HPP
