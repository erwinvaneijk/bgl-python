// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <boost/graph/python/python_property_map.hpp>
#include <boost/graph/python/resizable_property_map.hpp>
#include <boost/graph/python/property_map.hpp>
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/point3d.hpp>
#include <boost/graph/python/iterator.hpp> // for type_already_registered
#include <string>

namespace boost { namespace python {

// From http://aspn.activestate.com/ASPN/Mail/Message/C++-sig/1662717
template <class Extractor> 
struct lvalue_from_nonconst_pytype
{
  lvalue_from_nonconst_pytype(PyTypeObject *type)
  {
    // assume this is called only once
    m_type = type;
    converter::registry::insert(
      &extract, detail::extractor_type_id(&Extractor::execute));
  }
private:
  static PyTypeObject *m_type;

  static void* extract(PyObject* op)
  {
    return PyObject_TypeCheck(op, m_type)
             ? const_cast<void*> (
                 static_cast<void const volatile*> (
                    detail::normalize<Extractor> (&Extractor::execute).
                      execute(op)))
             : 0
      ;
  }
};

template <class Extractor>  PyTypeObject *
    lvalue_from_nonconst_pytype<Extractor> ::m_type = 0;
} } // end namespace boost::python

namespace boost { namespace graph { namespace python {

using boost::python::object;

template<typename Graph>
boost::python::object
add_vertex_property(Graph& g, const std::string& name, const std::string& type)
{
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;

  typedef python_property_map<vertex_index_t, Graph> result_type;
  result_type result(&g, type.c_str());

  boost::python::object result_obj(result);
  
  // Register named property maps
  if (!name.empty())
    g.vertex_properties()[name] = result_obj;

  return result_obj;
}

template<typename Graph>
boost::python::object
add_edge_property(Graph& g, const std::string& name, const std::string& type)
{
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;

  typedef python_property_map<edge_index_t, Graph> result_type;
  result_type result(&g, type.c_str());

  boost::python::object result_obj(result);
  
  // Register named property maps
  if (!name.empty())
    g.edge_properties()[name] = result_obj;

  return result_obj;
}

template<typename PythonPropertyMap, typename CppPropertyMap>
struct astype_property_map_extractor
{
  static CppPropertyMap& execute(PyObject& obj)
  {
    return boost::python::extract<PythonPropertyMap&>(&obj)();
  }
};

static const char* property_map_type_doc = 
  "type(self) -> str\n\n"
  "Returns the type of data stored in the property map.";

static const char* property_map_astype_doc = 
  "astype(self, type) -> object\n\n"
  "Returns a new property map that contains the same values as the\n"
  "property map self, but converted to the given type.\n";

static const char* property_map_address_doc = 
  "address(self) -> integer\n\n"
  "Returns the address of the raw data stored in the property map.\n"
  "To determine the type of this data, query the type method of\n"
  "the property map, then look up the corresponding C++ type in\n"
  "add_vertex_property and add_edge_property. The address points to\n"
  "the beginning of a contiguous array of property values, which can\n"
  "be indexed with the values in the `index' property map.\n";

template<typename Graph> 
void export_property_maps(const char* graph_name)
{
  using boost::graph::python::readable_property_map;
  using boost::graph::python::read_write_property_map;
  using boost::graph::python::lvalue_property_map;

  using boost::python::arg;
  using boost::python::class_;
  using boost::python::lvalue_from_nonconst_pytype;
  using boost::python::no_init;

  using boost::graph::python::detail::type_already_registered;

  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
  typedef typename property_map<Graph, vertex_index_t>::const_type 
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type EdgeIndexMap;

  // Make vertex property map available in Python
  typedef python_property_map<vertex_index_t, Graph> VertexPropertyMap;
  std::string vpm_name(graph_name);
  vpm_name += "VertexPropertyMap";
  class_<VertexPropertyMap> vertex_property_map(vpm_name.c_str(), no_init);
  read_write_property_map<VertexPropertyMap> vertex_reflect(vertex_property_map);
  vertex_property_map.def("astype", &VertexPropertyMap::astype,
                          (arg("property_map"), arg("type")),
                          property_map_astype_doc);
  vertex_property_map.def("type", &VertexPropertyMap::type, 
                          arg("property_map"),
                          property_map_type_doc);
  vertex_property_map.def("address", &VertexPropertyMap::address,
                          arg("property_map"),
                          property_map_address_doc);

  // Make edge property map available in Python
  typedef python_property_map<edge_index_t, Graph> EdgePropertyMap;
  std::string epm_name(graph_name);
  epm_name += "EdgePropertyMap";
  class_<EdgePropertyMap> edge_property_map(epm_name.c_str(), no_init);
  read_write_property_map<EdgePropertyMap> edge_reflect(edge_property_map);
  edge_property_map.def("astype", &EdgePropertyMap::astype,
                        (arg("property_map"), arg("type")),
                        property_map_astype_doc);
  edge_property_map.def("type", &EdgePropertyMap::type, 
                        arg("property_map"),
                        property_map_type_doc);
  edge_property_map.def("address", &EdgePropertyMap::address,
                        arg("property_map"),
                        property_map_address_doc);

  // Make implicit conversions from the vertex and edge property maps
  // to the associated vector_property_maps.
#define VERTEX_PROPERTY(Name,Type,Kind)                 \
  lvalue_from_nonconst_pytype<                          \
    astype_property_map_extractor<                      \
      VertexPropertyMap,                                \
      vector_property_map<Type, VertexIndexMap> > >     \
    BOOST_JOIN(Vertex,BOOST_JOIN(Name,PropertyMap))(    \
     (PyTypeObject*)vertex_property_map.ptr());
#define EDGE_PROPERTY(Name,Type,Kind)                   \
  lvalue_from_nonconst_pytype<                          \
    astype_property_map_extractor<                      \
      EdgePropertyMap,                                  \
      vector_property_map<Type, EdgeIndexMap> > >       \
    BOOST_JOIN(Edge,BOOST_JOIN(Name,PropertyMap))(      \
     (PyTypeObject*)edge_property_map.ptr());
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY

  // Reflect the underlying vector_property_maps into Python. This is
  // needed only to handle the default-value case (where we use NULL
  // pointers).
#define VERTEX_PROPERTY(Name,Type,Kind)                                 \
  class_<vector_property_map< Type, VertexIndexMap> >("", no_init);
#define EDGE_PROPERTY(Name,Type,Kind)                                   \
  class_<vector_property_map< Type, EdgeIndexMap> >("", no_init);
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY
}

// Explicit instantiations for the graph types we're interested in
#define INSTANTIATE_FOR_GRAPH(Name,Type)                                \
  template void export_property_maps< Type >(const char*);              \
  template object add_vertex_property< Type >(Type & g,                 \
                                              const std::string& name,  \
                                              const std::string& type); \
  template object add_edge_property< Type >(Type & g,                   \
                                              const std::string& name,  \
                                              const std::string& type);
#ifdef DIRECTED_PROPERTIES_ONLY
#define UNDIRECTED_GRAPH(Name,Type)
#define DIRECTED_GRAPH(Name,Type) INSTANTIATE_FOR_GRAPH(Name,Type)
#else
#define UNDIRECTED_GRAPH(Name,Type) INSTANTIATE_FOR_GRAPH(Name,Type)
#define DIRECTED_GRAPH(Name,Type)
#endif
#include "graphs.hpp"

} } } // end namespace boost::graph::python
