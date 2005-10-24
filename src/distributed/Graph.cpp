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
#include <boost/graph/python/generators.hpp>
#include <boost/graph/python/property_map.hpp>
#include <boost/vector_property_map.hpp>
#include <boost/graph/python/point2d.hpp>
#include <string>
#include <boost/parallel/owner_property_map.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

using boost::python::object;
using boost::python::class_;

template<typename Graph>
object vertex_property_map(const Graph& g, const std::string type)
{
  using boost::graph::python::point2d;
  using boost::parallel::owner_property_map;

  typedef typename property_map<Graph, vertex_index_t>::type IndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor    Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor      Edge;
  typedef typename Graph::process_group_type process_group_type;
  typedef typename process_group_type::process_id_type process_id_type;

  if (type == "int")
    return object(vector_property_map<int, IndexMap>(num_vertices(g),
                                                     get(vertex_index, g)));
  else if (type == "float")
    return object(vector_property_map<float, IndexMap>(num_vertices(g),
                                                       get(vertex_index, g)));
  else if (type == "vertex")
    return object(vector_property_map<Vertex, IndexMap>(num_vertices(g),
                                                        get(vertex_index, g)));
  else if (type == "edge")
    return object(vector_property_map<Edge, IndexMap>(num_vertices(g),
                                                      get(vertex_index, g)));
  else if (type == "string")
    return object(vector_property_map<std::string, IndexMap>
                  (num_vertices(g), get(vertex_index, g)));
  else if (type == "point2d")
    return object(vector_property_map<point2d, IndexMap>
                  (num_vertices(g), get(vertex_index, g)));
  else if (type == "color")
    return object(vector_property_map<default_color_type, 
                                      IndexMap>(num_vertices(g),
                                                get(vertex_index, g)));
  else if (type == "owner")
    return object(owner_property_map<Vertex, process_id_type>());
  else
    return object();
}

template<typename Graph>
object edge_property_map(const Graph& g, const std::string type)
{
  using boost::graph::python::point2d;
  using boost::parallel::owner_property_map;

  typedef typename property_map<Graph, edge_index_t>::type IndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor    Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor      Edge;
  typedef typename Graph::process_group_type process_group_type;
  typedef typename process_group_type::process_id_type process_id_type;

  if (type == "int")
    return object(vector_property_map<int, IndexMap>(num_edges(g),
                                                     get(edge_index, g)));
  else if (type == "float")
    return object(vector_property_map<float, IndexMap>(num_edges(g),
                                                       get(edge_index, g)));
  else if (type == "vertex")
    return object(vector_property_map<Vertex, IndexMap>(num_edges(g),
                                                        get(edge_index, g)));
  else if (type == "edge")
    return object(vector_property_map<Edge, IndexMap>(num_edges(g),
                                                      get(edge_index, g)));
  else if (type == "string")
    return object(vector_property_map<std::string, IndexMap>
                  (num_edges(g), get(edge_index, g)));
  else if (type == "point2d")
    return object(vector_property_map<point2d, IndexMap>
                  (num_edges(g), get(edge_index, g)));
  else if (type == "owner")
    return object(owner_property_map<Edge, process_id_type>());
  else
    return object();
}

template<typename T>
void do_synchronize(T& t)
{
  synchronize(t);
}

template<typename Graph>
void export_subgraph(class_<Graph, noncopyable>& graph);

void export_Graph()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::manage_new_object;
  using boost::python::no_init;
  using boost::python::return_value_policy;
  using boost::python::scope;

  using boost::graph::python::readable_property_map;
  using boost::graph::python::read_write_property_map;
  using boost::graph::python::lvalue_property_map;

  using boost::graph::python::point2d;

  using boost::graph::python::detail::type_already_registered;

  typedef graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef graph_traits<Graph>::edge_descriptor edge_descriptor;

  {
    scope s;
    class_<Graph, noncopyable> graph("Graph");

    // Graph concepts
    boost::graph::python::graph<Graph> g(graph);
    boost::graph::python::vertex_list_graph<Graph> vlg(graph);
    boost::graph::python::edge_list_graph<Graph> elg(graph);
    boost::graph::python::incidence_graph<Graph> ig(graph);
    boost::graph::python::bidirectional_graph<Graph> bg(graph);
    boost::graph::python::adjacency_graph<Graph> ag(graph);
    boost::graph::python::mutable_graph<Graph, false, true> mg(graph);

    // Synchronization
    graph.def("synchronize", &do_synchronize<Graph>);

    // Subgraphs
    export_subgraph(graph);

    // Property maps
    typedef property_map<Graph, vertex_index_t>::type VertexIndexMap;
    typedef property_map<Graph, edge_index_t>::type EdgeIndexMap;
    typedef graph_traits<Graph>::vertex_descriptor    Vertex;
    typedef graph_traits<Graph>::edge_descriptor      Edge;
    typedef process_group_type::process_id_type process_id_type;

    typedef vector_property_map<int, VertexIndexMap> VertexIntMap;
    if (!type_already_registered<VertexIntMap>()) {
      class_<VertexIntMap> pm("VertexIntMap", no_init);
      read_write_property_map<VertexIntMap> rwpm(pm);
      pm.def("synchronize", &VertexIntMap::do_synchronize);
    }

    typedef vector_property_map<float, VertexIndexMap> VertexFloatMap;
    if (!type_already_registered<VertexFloatMap>()) {
      class_<VertexFloatMap> pm("VertexFloatMap", no_init);
      read_write_property_map<VertexFloatMap> rwpm(pm);
      pm.def("synchronize", &VertexFloatMap::do_synchronize);
    }

    typedef vector_property_map<Vertex, VertexIndexMap> VertexVertexMap;
    if (!type_already_registered<VertexVertexMap>()) {
      class_<VertexVertexMap> pm("VertexVertexMap", no_init);
      read_write_property_map<VertexVertexMap> rwpm(pm);
      pm.def("synchronize", &VertexVertexMap::do_synchronize);
    }

    typedef vector_property_map<default_color_type, VertexIndexMap>
      VertexColorMap;
    if (!type_already_registered<VertexColorMap>()) {
      class_<VertexColorMap> pm("VertexColorMap", no_init);
      read_write_property_map<VertexColorMap> rwpm(pm);
      pm.def("synchronize", &VertexColorMap::do_synchronize);
    }

    typedef boost::parallel::owner_property_map<Vertex, process_id_type>
      VertexOwnerMap;
    if (!type_already_registered<VertexOwnerMap>()) {
      class_<VertexOwnerMap> pm("VertexOwnerMap", no_init);
      readable_property_map<VertexOwnerMap> rpm(pm);
    }

    typedef boost::parallel::owner_property_map<Edge, process_id_type>
      EdgeOwnerMap;
    if (!type_already_registered<EdgeOwnerMap>()) {
      class_<EdgeOwnerMap> pm("EdgeOwnerMap", no_init);
      readable_property_map<EdgeOwnerMap> rpm(pm);
    }

    typedef vector_property_map<Edge, VertexIndexMap> VertexEdgeMap;
    if (!type_already_registered<VertexEdgeMap>()) {
      class_<VertexEdgeMap> pm("VertexEdgeMap", no_init);
      read_write_property_map<VertexEdgeMap> rwpm(pm);
      pm.def("synchronize", &VertexEdgeMap::do_synchronize);
    }

    typedef vector_property_map<std::string, VertexIndexMap> VertexStringMap;
    if (!type_already_registered<VertexStringMap>()) {
      class_<VertexStringMap> vsm("VertexStringMap", no_init);
      lvalue_property_map<VertexStringMap> lpm(vsm);
      vsm.def("synchronize", &VertexStringMap::do_synchronize);
    }

    typedef vector_property_map<point2d, VertexIndexMap> VertexPoint2DMap;
    if (!type_already_registered<VertexPoint2DMap>()) {
      class_<VertexPoint2DMap> vsm("VertexPoint2DMap", no_init);
      lvalue_property_map<VertexPoint2DMap> lpm(vsm);
      vsm.def("synchronize", &do_synchronize<VertexPoint2DMap>);
    }

    // Properties
    graph.def("vertex_property_map", &vertex_property_map<Graph>,
              (arg("graph"), arg("type")));
  }

  // Generators
  def("erdos_renyi_graph",
      &boost::graph::python::erdos_renyi_graph<Graph>,
      return_value_policy<manage_new_object>(),
      (arg("num_vertices"), arg("probability"),
       arg("allow_self_loops") = false, arg("random_seed") = 1));

  def("plod_graph",
      &boost::graph::python::plod_graph<Graph>,
      return_value_policy<manage_new_object>(),
      (arg("num_vertices"), arg("alpha"), arg("beta"),
       arg("allow_self_loops") = false, arg("random_seed") = 1));

  def("small_world_graph",
      &boost::graph::python::small_world_graph<Graph>,
      return_value_policy<manage_new_object>(),
      (arg("num_vertices"), arg("num_neighbors"), arg("rewire_probability"),
       arg("allow_self_loops") = false, arg("random_seed") = 1));
}

} } } } // end namespace boost::graph::distributed::python
