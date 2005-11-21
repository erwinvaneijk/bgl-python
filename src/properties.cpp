// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <boost/graph/python/property_map.hpp>
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/point3d.hpp>
#include <boost/graph/python/iterator.hpp> // for type_already_registered
#include <string>
#include <boost/graph/python/resizable_property_map.hpp>

namespace boost { namespace graph { namespace python {

using boost::python::object;

template<typename Graph>
object vertex_property_map(Graph& g, const std::string& type)
{
  typedef typename property_map<Graph, vertex_index_t>::const_type IndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;

  if (type == "index")
    return object(get(vertex_index, g));
#define VERTEX_PROPERTY(Name,Type,Kind)                                 \
  else if (type == #Name) {                                             \
    typedef vector_property_map<Type, IndexMap> pmap_type;              \
    typedef resizable_vector_property_map<Type, IndexMap> resize_pmap_type; \
    pmap_type pmap(num_vertices(g), get(vertex_index, g));              \
    std::auto_ptr<resizable_property_map> reg(new resize_pmap_type(pmap)); \
    g.register_vertex_map(reg);                                         \
    return object(pmap);                                                \
  }
#define EDGE_PROPERTY(Name,Type,Kind)
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY
  else
    return object();
}

template<typename Graph>
object edge_property_map(Graph& g, const std::string& type)
{
  typedef typename property_map<Graph, edge_index_t>::const_type IndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;

  if (type == "index")
    return object(get(edge_index, g));
#define VERTEX_PROPERTY(Name,Type,Kind)
#define EDGE_PROPERTY(Name,Type,Kind)                                 \
  else if (type == #Name) {                                             \
    typedef vector_property_map<Type, IndexMap> pmap_type;              \
    typedef resizable_vector_property_map<Type, IndexMap> resize_pmap_type; \
    pmap_type pmap(num_edges(g), get(edge_index, g));              \
    std::auto_ptr<resizable_property_map> reg(new resize_pmap_type(pmap)); \
    g.register_edge_map(reg);                                         \
    return object(pmap);                                                \
  }
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY
  else
    return object();
}

template<typename Graph> 
void export_property_maps()
{
  using boost::graph::python::readable_property_map;
  using boost::graph::python::read_write_property_map;
  using boost::graph::python::lvalue_property_map;

  using boost::python::class_;
  using boost::python::no_init;

  using boost::graph::python::detail::type_already_registered;

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
    }                                                                   \
  }
#define EDGE_PROPERTY(Name,Type,Kind)                                   \
  {                                                                     \
    typedef vector_property_map<Type, EdgeIndexMap> EdgeMap;            \
    if (!type_already_registered<EdgeMap>()) {                          \
      class_<EdgeMap> pm("Edge" #Name "Map", no_init);                  \
      BOOST_JOIN(Kind,_property_map)<EdgeMap> reflect_pm(pm);           \
    }                                                                   \
  }
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY_MAP
#undef VERTEX_PROPERTY_MAP

  if (!type_already_registered<VertexIndexMap>()) {
    class_<VertexIndexMap> pm("VertexIndexMap", no_init);
    readable_property_map<VertexIndexMap> rpm(pm);
  }

  if (!type_already_registered<EdgeIndexMap>()) {
    class_<EdgeIndexMap> pm("EdgeIndexMap", no_init);
    readable_property_map<EdgeIndexMap> rpm(pm);
  }
}

// Explicit instantiations for the graph types we're interested in
#define UNDIRECTED_GRAPH(Name,Type)                                     \
  template void export_property_maps< Type >();                         \
  template object vertex_property_map< Type >(Type & g,                 \
                                              const std::string& type);  \
  template object edge_property_map< Type >(Type & g,                   \
                                            const std::string& type);
#include "graphs.hpp"

} } } // end namespace boost::graph::python
