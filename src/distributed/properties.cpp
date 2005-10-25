// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <boost/parallel/property_map.hpp>
#include <boost/graph/python/property_map.hpp>
#include <boost/parallel/owner_property_map.hpp>
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/point3d.hpp>
#include <boost/parallel/container_traits.hpp>
#include <string>

namespace boost { namespace graph { namespace distributed { namespace python {

using boost::python::object;

template<typename Graph>
object vertex_property_map(const Graph& g, const std::string type)
{
  using boost::parallel::owner_property_map;

  typedef typename property_map<Graph, vertex_index_t>::const_type IndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename Graph::process_group_type process_group_type;
  typedef typename process_group_type::process_id_type process_id_type;

  if (type == "owner")
    return object(owner_property_map<vertex_descriptor, process_id_type>());

#define VERTEX_PROPERTY(Name,Type,Kind)                                 \
  else if (type == #Name)                                               \
    return object(vector_property_map<Type, IndexMap>(num_vertices(g),  \
                                                      get(vertex_index, g)));
#define EDGE_PROPERTY(Name,Type,Kind)
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY_MAP
#undef VERTEX_PROPERTY_MAP
  else
    return object();
}

template<typename Graph>
object edge_property_map(const Graph& g, const std::string type)
{
  using boost::parallel::owner_property_map;

  typedef typename property_map<Graph, edge_index_t>::const_type IndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename Graph::process_group_type process_group_type;
  typedef typename process_group_type::process_id_type process_id_type;

  if (type == "owner")
    return object(owner_property_map<edge_descriptor, process_id_type>());

#define VERTEX_PROPERTY(Name,Type,Kind)
#define EDGE_PROPERTY(Name,Type,Kind)                                   \
  else if (type == #Name)                                               \
    return object(vector_property_map<Type, IndexMap>(num_edges(g),     \
                                                      get(edge_index, g)));
  #include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY_MAP
#undef VERTEX_PROPERTY_MAP
  else
    return object();
}

template<typename Graph> 
void export_property_maps()
{
  using boost::graph::python::detail::type_already_registered;

  using boost::graph::python::readable_property_map;
  using boost::graph::python::read_write_property_map;
  using boost::graph::python::lvalue_property_map;

  using boost::python::class_;
  using boost::python::no_init;

  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename property_map<Graph, vertex_index_t>::const_type 
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type EdgeIndexMap;

#define VERTEX_PROPERTY(Name,Type,Kind)                                 \
  {                                                                     \
    typedef vector_property_map<Type, VertexIndexMap> VertexMap;        \
    if (!type_already_registered<VertexMap>()) {                        \
      class_<VertexMap> pm("Vertex" #Name "Map", no_init);              \
      BOOST_JOIN(Kind,_property_map)<VertexMap> reflect_pm(pm);         \
      pm.def("synchronize", &VertexMap::do_synchronize);                \
    }                                                                   \
  }
#define EDGE_PROPERTY(Name,Type,Kind)                                   \
  {                                                                     \
    typedef vector_property_map<Type, EdgeIndexMap> EdgeMap;            \
    if (!type_already_registered<EdgeMap>()) {                          \
      class_<EdgeMap> pm("Edge" #Name "Map", no_init);                  \
      BOOST_JOIN(Kind,_property_map)<EdgeMap> reflect_pm(pm);           \
      pm.def("synchronize", &EdgeMap::do_synchronize);                  \
    }                                                                   \
  }
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY_MAP
#undef VERTEX_PROPERTY_MAP

  typedef typename boost::parallel::process_group_type<Graph>::type
    process_group_type;
  typedef typename process_group_type::process_id_type process_id_type;

  typedef boost::parallel::owner_property_map<vertex_descriptor, 
                                              process_id_type>
    VertexOwnerMap;
  if (!type_already_registered<VertexOwnerMap>()) {
    class_<VertexOwnerMap> pm("VertexOwnerMap", no_init);
    readable_property_map<VertexOwnerMap> rpm(pm);
  }

  typedef boost::parallel::owner_property_map<edge_descriptor, process_id_type>
    EdgeOwnerMap;
  if (!type_already_registered<EdgeOwnerMap>()) {
    class_<EdgeOwnerMap> pm("EdgeOwnerMap", no_init);
    readable_property_map<EdgeOwnerMap> rpm(pm);
  }
}

// Explicit instantiations for the graph types we're interested in
#define UNDIRECTED_GRAPH(Name,Type)                                     \
  template void export_property_maps< Type >();                         \
  template object vertex_property_map< Type >(const Type & g,           \
                                              const std::string type);  \
  template object edge_property_map< Type >(const Type & g,             \
                                            const std::string type);
#include "graphs.hpp"

} } } } // end namespace boost::graph::distributed::python
