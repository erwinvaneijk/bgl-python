// Copyright 2005 The Trustees of Indiana University.

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

namespace boost { namespace graph { namespace python {

class python_dynamic_property_map
{
 public:
  virtual ~python_dynamic_property_map() {}

  virtual void copy_value(const any& to, const any& from) = 0;
  virtual boost::python::object get_python(const any& key) = 0;
};

template<typename PropertyMap,
         typename ValueType = typename property_traits<PropertyMap>::value_type>
class python_dynamic_adaptor
  : public boost::detail::dynamic_property_map_adaptor<PropertyMap>, 
    public python_dynamic_property_map
{
  typedef boost::detail::dynamic_property_map_adaptor<PropertyMap> inherited;

public:
  typedef typename property_traits<PropertyMap>::key_type key_type;

  explicit python_dynamic_adaptor(const PropertyMap& property_map)
    : inherited(property_map) { }

  virtual void copy_value(const any& to, const any& from)
  { 
    boost::put(this->base(), any_cast<key_type>(to), 
               boost::get(this->base(), any_cast<key_type>(from)));
  }

  virtual boost::python::object get_python(const any& key)
  {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
    return boost::get(this->base(), any_cast<key_type>(key));
#else
    using boost::get;

    return boost::python::object(get(this->base(), any_cast<key_type>(key)));
#endif    
  }
};

template<typename PropertyMap>
class python_dynamic_adaptor<PropertyMap, boost::python::object>
  : public boost::detail::dynamic_property_map_adaptor<PropertyMap>, 
    public python_dynamic_property_map
{
  typedef boost::detail::dynamic_property_map_adaptor<PropertyMap> inherited;

public:
  typedef typename property_traits<PropertyMap>::key_type key_type;

  explicit python_dynamic_adaptor(const PropertyMap& property_map)
    : inherited(property_map) { }

  virtual void copy_value(const any& to, const any& from)
  { 
    boost::put(this->base(), any_cast<key_type>(to), 
               boost::get(this->base(), any_cast<key_type>(from)));
  }

  virtual std::string get_string(const any& key)
  {
    using boost::python::extract;
    using boost::python::str;
    return std::string(
             extract<const char*>(str(boost::get(this->base(),
                                                 any_cast<key_type>(key)))));
  }

  virtual boost::python::object get_python(const any& key)
  {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
    return boost::get(this->base(), any_cast<key_type>(key));
#else
    using boost::get;

    return get(this->base(), any_cast<key_type>(key));
#endif    
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
      typedef vector_property_map<std::string, VertexIndexMap>
        property_map_type;
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset
        (new adaptor_type(property_map_type(num_vertices(*g),
                                            get(vertex_index, *g))));
    } else if (key.type() == typeid(Edge)) {
      typedef vector_property_map<std::string, EdgeIndexMap> property_map_type;
      typedef python_dynamic_adaptor<property_map_type> adaptor_type;
      result.reset
        (new adaptor_type(property_map_type(num_edges(*g),
                                            get(edge_index, *g))));
    } 
    return result;
  }

private:
  Graph* g;
};

// Converts a dynamic_properties structure containing only
// vertex->string and edge->string property maps into a Python
// dictionary. This requires us to transform the C++ std::strings into
// Python strings.
//
// DPG TBD: Figure out why we can't just use boost::python::str as our
// string in read_graphviz. Then we could skip this silly translation
// step.
template<typename Graph>
void 
string_properties_to_dicts(Graph& g, const dynamic_properties& in)
{
  using boost::python::str;

  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;
  
  typedef vector_property_map<std::string, VertexIndexMap> InVertexStringMap;
  typedef vector_property_map<std::string, EdgeIndexMap> InEdgeStringMap;
  typedef vector_property_map<str, VertexIndexMap> OutVertexStringMap;
  typedef vector_property_map<str, EdgeIndexMap> OutEdgeStringMap;
  typedef boost::detail::dynamic_property_map_adaptor<InVertexStringMap>
    VertexAdaptor;
  typedef boost::detail::dynamic_property_map_adaptor<InEdgeStringMap>
    EdgeAdaptor;

  for (dynamic_properties::const_iterator i = in.begin(); i != in.end(); ++i) {
    if (VertexAdaptor* adaptor = dynamic_cast<VertexAdaptor*>(i->second)) {
      InVertexStringMap in_pmap = adaptor->base();

      // Build the new property map
      OutVertexStringMap out_pmap(num_vertices(g), get(vertex_index, g));
      BGL_FORALL_VERTICES_T(v, g, Graph)
        out_pmap[v] = str(in_pmap[v]);

      // Register the new property map
      typedef resizable_vector_property_map<str, VertexIndexMap>
        resize_pmap_type;
      std::auto_ptr<resizable_property_map> reg(new resize_pmap_type(out_pmap));
      g.register_vertex_map(reg);

      g.vertex_properties()[i->first] = out_pmap;
    } else if (EdgeAdaptor* adaptor = dynamic_cast<EdgeAdaptor*>(i->second)) {
      InEdgeStringMap in_pmap = adaptor->base();

      // Build the new property map
      OutEdgeStringMap out_pmap(num_edges(g), get(edge_index, g));
      BGL_FORALL_EDGES_T(e, g, Graph)
        out_pmap[e] = str(in_pmap[e]);

      // Register the new property map
      typedef resizable_vector_property_map<str, EdgeIndexMap>
        resize_pmap_type;
      std::auto_ptr<resizable_property_map> reg(new resize_pmap_type(out_pmap));
      g.register_edge_map(reg);      

      g.edge_properties()[i->first] = out_pmap;
    } else {
      // Skipping a property (!)
    }
  }
}

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
