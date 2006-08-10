// Copyright (C) 2005, 2006 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

#ifndef BOOST_GRAPH_PYTHOH_DYNAMIC_PROPERTIES_HPP
#define BOOST_GRAPH_PYTHOH_DYNAMIC_PROPERTIES_HPP

#include <boost/dynamic_property_map.hpp>
#include <boost/python.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/vector_property_map.hpp>
#include <string>
#include <memory>
#include <boost/graph/python/resizable_property_map.hpp>
#include <boost/graph/python/python_property_map.hpp>

namespace boost { namespace graph { namespace python {

template<typename PropertyMap>
class python_dynamic_adaptor
  : public boost::detail::dynamic_property_map_adaptor<PropertyMap>
{
  typedef boost::detail::dynamic_property_map_adaptor<PropertyMap> inherited;

public:
  typedef typename property_traits<PropertyMap>::key_type key_type;
  typedef typename property_traits<PropertyMap>::value_type value_type;

  explicit python_dynamic_adaptor(const PropertyMap& property_map)
    : inherited(property_map) { }

  virtual std::string get_string(const any& key)
  {
    using boost::python::extract;
    using boost::python::str;
    using boost::get;

    return extract<const char*>(str(get(this->base(), any_cast<key_type>(key))))();
  }

  virtual void put(const any& key, const any& value)
  {
    using boost::put;
    using boost::graph::python::put;
    using boost::python::object;
    using boost::python::str;

    if (const str* str_ptr = any_cast<str>(&value))
      put(this->base(), any_cast<key_type>(key), *str_ptr);
    else if (const std::string* str_ptr = any_cast<std::string>(&value))
      put(this->base(), any_cast<key_type>(key), str(str_ptr->c_str()));
    else if (const bool* bool_ptr = any_cast<bool>(&value))
      put(this->base(), any_cast<key_type>(key), object(*bool_ptr));
    else if (const int* int_ptr = any_cast<int>(&value))
      put(this->base(), any_cast<key_type>(key), object(*int_ptr));
    else if (const long* long_ptr = any_cast<long>(&value))
      put(this->base(), any_cast<key_type>(key), object(*long_ptr));
    else if (const float* float_ptr = any_cast<float>(&value))
      put(this->base(), any_cast<key_type>(key), object(*float_ptr));
    else if (const double* double_ptr = any_cast<double>(&value))
      put(this->base(), any_cast<key_type>(key), object(*double_ptr));
    else
      assert(false);
  }
};

template<typename Graph>
struct build_string_property_maps
{
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor   Edge;

  build_string_property_maps(Graph* g) : g(g) { }

  std::auto_ptr<boost::dynamic_property_map>
  operator()(const std::string& name, const boost::any& key, 
             const boost::any& value)
  {
    std::auto_ptr<boost::dynamic_property_map> result(0);

    if (key.type() == typeid(Vertex)) {
      // Create property map and place it in the graph
      typedef python_property_map<vertex_index_t, Graph> property_map_type;
      property_map_type pmap(g, "string");
      g->vertex_properties()[name] = boost::python::object(pmap);

      // Build an entry for dynamic_properties
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset(new adaptor_type(pmap));
    } else if (key.type() == typeid(Edge)) {
      // Create property map and place it in the graph
      typedef python_property_map<edge_index_t, Graph> property_map_type;
      property_map_type pmap(g, "string");
      g->edge_properties()[name] = boost::python::object(pmap);

      // Build an entry for dynamic_properties
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset(new adaptor_type(pmap));
    }
    return result;
  }

private:
  Graph* g;
};

template<typename Graph>
struct build_python_property_maps
{
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor   Edge;

  build_python_property_maps(Graph* g) : g(g) { }

  std::auto_ptr<boost::dynamic_property_map>
  operator()(const std::string& name, const boost::any& key, 
             const boost::any& value)
  {
    std::auto_ptr<boost::dynamic_property_map> result(0);

    if (key.type() == typeid(Vertex)) {
      // Create property map and place it in the graph
      typedef python_property_map<vertex_index_t, Graph> property_map_type;
      property_map_type pmap(g, type_from_value(value));
      g->vertex_properties()[name] = boost::python::object(pmap);

      // Build an entry for dynamic_properties
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset(new adaptor_type(pmap));
    } else if (key.type() == typeid(Edge)) {
      // Create property map and place it in the graph
      typedef python_property_map<edge_index_t, Graph> property_map_type;
      property_map_type pmap(g, type_from_value(value));
      g->edge_properties()[name] = boost::python::object(pmap);

      // Build an entry for dynamic_properties
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset(new adaptor_type(pmap));
    }
    return result;
  }

  static const char* type_from_value(const any& value)
  {
    if (value.type() == typeid(bool))
      return "integer";
    else if (value.type() == typeid(int) || value.type() == typeid(long))
      return "integer";
    else if (value.type() == typeid(float) || value.type() == typeid(double))
      return "float";
    else if (value.type() == typeid(std::string))
      return "string";
    else
      return "string";
  }

private:
  Graph* g;
};

template<typename Key>
struct object_as_string_property_map
{
  typedef Key                        key_type;
  typedef std::string                value_type;
  typedef value_type                 reference;
  typedef readable_property_map_tag  category;

  explicit object_as_string_property_map(boost::python::object pmap) 
    : pmap(pmap) { }

  boost::python::object pmap;
};

template<typename Key>
std::string
get(object_as_string_property_map<Key> pmap, 
    typename object_as_string_property_map<Key>::key_type key)
{
  using boost::python::extract;
  return std::string(extract<const char*>(pmap.pmap[key]));
}

template<typename Key>
struct object_as_str_property_map
{
  typedef Key                        key_type;
  typedef boost::python::str         value_type;
  typedef value_type                 reference;
  typedef readable_property_map_tag  category;

  explicit object_as_str_property_map(boost::python::object pmap) 
    : pmap(pmap) { }

  boost::python::object pmap;
};

template<typename Key>
boost::python::str 
get(object_as_str_property_map<Key> pmap, 
    typename object_as_str_property_map<Key>::key_type key)
{
  return boost::python::str(pmap.pmap[key]);
}

// Converts a Python dictionary into a dynamic_properties structure.
template<typename Key>
void 
dict_to_properties(const boost::python::dict& in, dynamic_properties& out)
{
  using boost::python::object;
  using boost::python::extract;

  try {
    object iter = in.iteritems();
    while (true) {
      object item = iter.attr("next")();
      std::string name;
      name = extract<const char*>(item[0]);
      out.property(name, object_as_str_property_map<Key>(item[1]));
    }
  } catch (...) {
    if (PyErr_Occurred() && PyErr_ExceptionMatches(PyExc_StopIteration)) {
      // Swallow the exception
      PyErr_Clear();
    } else {
      throw;
    }
  }
}

} } } // end namespace boost::graph::python

namespace boost {
  using boost::graph::python::get;
} 

#endif // BOOST_GRAPH_PYTHOH_DYNAMIC_PROPERTIES_HPP
