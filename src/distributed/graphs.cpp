// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/parallel/property_map.hpp>
#include <boost/graph/python/graph.hpp>
#include <boost/graph/python/property_map.hpp>
#include <boost/vector_property_map.hpp>
#include "exports.hpp"

namespace boost { namespace graph { namespace distributed { namespace python {

using boost::python::object;
using boost::python::class_;

template<typename T>
void do_synchronize(T& t)
{
  synchronize(t);
}

template<typename Graph>
typename graph_traits<Graph>::vertex_descriptor
nth_vertex(const Graph& g, typename graph_traits<Graph>::vertices_size_type n)
{
  return vertex(n, g);
}

template<typename Graph>
void export_graph(const char* name, const char* directedness)
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::manage_new_object;
  using boost::python::no_init;
  using boost::python::return_value_policy;
  using boost::python::scope;

  {
    scope s;
    class_<Graph, noncopyable> graph(name);

    // Graph concepts
    boost::graph::python::graph<Graph> g(graph);
    boost::graph::python::vertex_list_graph<Graph> vlg(graph);
    boost::graph::python::edge_list_graph<Graph> elg(graph);
    boost::graph::python::incidence_graph<Graph> ig(graph);
    boost::graph::python::bidirectional_graph<Graph> bg(graph);
    boost::graph::python::adjacency_graph<Graph> ag(graph);
    boost::graph::python::mutable_graph<Graph, false, true> mg(graph);

    // Get the Nth vertex in the entire graph
    graph.def("vertex", &nth_vertex<Graph>);

    // Synchronization
    graph.def("synchronize", &do_synchronize<Graph>);

    // Properties
    export_property_maps<Graph>();
    graph.def("vertex_property_map", &vertex_property_map<Graph>,
              (arg("graph"), arg("type")));
    graph.def("edge_property_map", &edge_property_map<Graph>,
              (arg("graph"), arg("type")));
  }

  export_generators<Graph>(directedness);
  export_metis<Graph>(directedness);
  export_graphviz<Graph>(directedness);
}

#define UNDIRECTED_GRAPH(Name,Type) \
  template void export_graph< Type >(const char*, const char*);
#include "graphs.hpp"

} } } } // end namespace boost::graph::distributed::python
