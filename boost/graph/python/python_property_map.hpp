// Copyright (C) 2006 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_PYTHON_PROPERTY_MAP_HPP
#define BOOST_GRAPH_PYTHON_PYTHON_PROPERTY_MAP_HPP
#include <boost/python.hpp>
#include <boost/graph/python/resizable_property_map.hpp>
#include <boost/property_map.hpp>
#include <boost/vector_property_map.hpp>
#include <boost/graph/python/iterator.hpp>
#include <boost/graph/properties.hpp>
#include <cstring>
#include <boost/shared_ptr.hpp>
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/point3d.hpp>
#include <sstream>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <cctype>

namespace boost { namespace graph { namespace python {

  // Forward declarations to help GCC 3.3.x
  template<typename DirectedS> class basic_graph;
 
  template<typename DirectedS>
  typename basic_graph<DirectedS>::VertexIndexMap
  get(vertex_index_t, const basic_graph<DirectedS>& g);
 
  template<typename DirectedS>
  typename basic_graph<DirectedS>::EdgeIndexMap
  get(edge_index_t, const basic_graph<DirectedS>& g);
 
  template<typename PropertyTag, typename Graph>
  class python_property_map;

  /// The integer type that will be used for addresses
  typedef mpl::if_c<(sizeof(void*) == sizeof(int)), int, long>::type
    integer_type_for_addresses;

  /**
   * Exception thrown when an unsupported type name is provided to one
   * of the routines in @c python_property_map.
   */
  struct bad_property_type : std::exception {
    explicit bad_property_type(const char* name) {
      msg = std::string("invalid property type name `") + name + "'";
    }

    virtual ~bad_property_type() throw() { }

    const char* what() const throw() { return msg.c_str(); }

  protected:
    std::string msg;
  };

  /**
   * Exception thrown when an invalid property conversion is
   * requested.
   */
  struct invalid_property_conversion : std::exception {
    explicit invalid_property_conversion(const char* from, const char* to) {
      msg = std::string("cannot convert properties of type `") + from 
        + "' to properties of type `" + to + "'";
    }

    virtual ~invalid_property_conversion() throw() { }

    const char* what() const throw() { return msg.c_str(); }

  protected:
    std::string msg;
  };

  /**
   * Exception thrown when we are unable to perform a property
   * conversion because, e.g., we can't parse a string.
   */
  struct cannot_parse_property : std::exception {
    explicit cannot_parse_property(const char* from, const char* type) {
      msg = std::string("cannot convert string `") + from + 
        "' to a value of type `" + type + "'";
    }

    virtual ~cannot_parse_property() throw() { }

    const char* what() const throw() { return msg.c_str(); }

  protected:
    std::string msg;
  };

  namespace detail {

    template<typename vertex_descriptor, typename edge_descriptor>
    struct property_value_type_name_impl {
#define VERTEX_PROPERTY(Name,Type,Kind)                 \
      static const char* name(Type) { return #Name; }
#define EDGE_PROPERTY(Name,Type,Kind)
#include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY      
    };

    template<typename T, typename vertex_descriptor, typename edge_descriptor>
    class property_value_type_name { 
      typedef property_value_type_name_impl<vertex_descriptor, edge_descriptor>
        impl;

    public:
      static const char* name() { return impl::name(T()); }
    };

    // The identity conversion
    template<typename T>
    inline const T& 
    py_convert(const T& from, type<T>) 
    {
      return from;
    }

    // Convert to a Python object
    template<typename From>
    inline boost::python::object 
    py_convert(const From& from, type<boost::python::object>) 
    {
      return boost::python::object(from);
    }
    
    // Convert from a Python object
    template<typename To>
    inline To
    py_convert(const boost::python::object& from, type<To>)
    {
      if (from == boost::python::object())
        return To();
      else
        return boost::python::extract<To>(from)();
    }

    // Tie-breaker: Python object identity function
    inline boost::python::object
    py_convert(const boost::python::object& from, type<boost::python::object>)
    {
      return from;
    }

    // int -> float
    inline float
    py_convert(const int& from, type<float>)
    {
      return from;
    }

    // float -> int
    inline int
    py_convert(const float& from, type<int>)
    {
      return (int)from;
    }

    // point2d -> point3d
    inline point3d
    py_convert(const point2d& from, type<point3d>)
    {
      return point3d(from[0], from[1], 0);
    }

    // point3d -> point2d
    inline point2d
    py_convert(const point3d& from, type<point2d>)
    {
      return point2d(from[0], from[1]);
    }

    // int -> string
    inline boost::python::str
    py_convert(const int& from, type<boost::python::str>)
    {
      return lexical_cast<std::string>(from).c_str();
    }

    // float -> string
    inline boost::python::str
    py_convert(const float& from, type<boost::python::str>)
    {
      return lexical_cast<std::string>(from).c_str();
    }

    // point2d -> string
    inline boost::python::str
    py_convert(const point2d& from, type<boost::python::str>)
    {
      std::ostringstream out;
      out << '(' << from[0] << ", " << from[1] << ')';
      return out.str().c_str();
    }

    // point3d -> string
    inline boost::python::str
    py_convert(const point3d& from, type<boost::python::str>)
    {
      std::ostringstream out;
      out << '(' << from[0] << ", " << from[1] << ", " << from[2]<< ')';
      return out.str().c_str();
    }

    // color -> string
    inline boost::python::str
    py_convert(const default_color_type& from, type<boost::python::str>)
    {
      switch (from) {
      case white_color: return "white";
      case gray_color:  return "gray";
      case green_color: return "green";
      case red_color:   return "red";
      case black_color: return "black";
      }
      return "<unknown color value>";
    }

    // object -> string
    inline boost::python::str
    py_convert(const boost::python::object& from, type<boost::python::str>)
    {
      return boost::python::str(from);
    }

    // string -> int
    inline int
    py_convert(const boost::python::str& from, type<int>)
    {
      using boost::python::extract;
      const char* value = extract<const char*>(from)();
      try {
        return lexical_cast<int>(value);
      } catch (bad_lexical_cast) {
        throw cannot_parse_property(value, "integer");
      }
    }

    // string -> float
    inline float
    py_convert(const boost::python::str& from, type<float>)
    {
      using boost::python::extract;
      const char* value = extract<const char*>(from)();
      try {
        return lexical_cast<float>(value);
      } catch (bad_lexical_cast) {
        throw cannot_parse_property(value, "float");
      }
    }

    // string -> point2d
    inline point2d
    py_convert(const boost::python::str& from, type<point2d>)
    {
      using boost::python::extract;
      point2d result;
      char c;
      const char* text = extract<const char*>(from)();
      std::istringstream in(text);
      using std::isspace;

      try {
        // Skip over spaces
        while (isspace(in.peek())) { in.read(&c, 1); }
        
        // Recognize various starting characters: [ (, or nothing
        if (in.peek() == '[' || in.peek() == '(')
          in.read(&c, 1);
        
        // Get the "x" coordinate
        if (!(in >> result[0]))
          throw bad_lexical_cast();
        
        // Skip over spaces
        while (isspace(in.peek())) { in.read(&c, 1); }
        
        // Recognize a separator (',') if we see one
        if (in.peek() == ',')
          in.read(&c, 1);
        
        // Get the "y" coordinate
        if (!(in >> result[1]))
          throw bad_lexical_cast();
        
        // Good enough: we're done.
        return result;
      } catch (bad_lexical_cast) {
        throw cannot_parse_property(text, "point2d");
      }
    }

    // string -> point3d
    inline point3d
    py_convert(const boost::python::str& from, type<point3d>)
    {
      using boost::python::extract;
      point3d result;
      char c;
      const char* text = extract<const char*>(from)();
      std::istringstream in(text);
      using std::isspace;

      try {
        // Skip over spaces
        while (isspace(in.peek())) { in.read(&c, 1); }

        // Recognize various starting characters: [ (, or nothing
        if (in.peek() == '[' || in.peek() == '(')
          in.read(&c, 1);

        // Get the "x" coordinate
        if (!(in >> result[0]))
          throw bad_lexical_cast();

        // Skip over spaces
        while (isspace(in.peek())) { in.read(&c, 1); }

        // Recognize a separator (',') if we see one
        if (in.peek() == ',')
          in.read(&c, 1);
      
        // Get the "y" coordinate
        if (!(in >> result[1]))
          throw bad_lexical_cast();

        // Recognize a separator (',') if we see one
        if (in.peek() == ',')
          in.read(&c, 1);
      
        // Get the "z" coordinate
        if (!(in >> result[2]))
          throw bad_lexical_cast();

        // Good enough: we're done.
        return result;
      } catch (bad_lexical_cast) {
        throw cannot_parse_property(text, "point3d");
      }
    }

    // string -> color
    inline default_color_type
    py_convert(const boost::python::str& from, type<default_color_type>)
    {
      using boost::python::extract;
      const char* raw_text = extract<const char*>(from)();
      std::istringstream in(raw_text);
      std::string text;
      try {
        if (in >> text) {
          if (text == "white") return white_color;
          else if (text == "gray") return gray_color;
          else if (text == "green") return green_color;
          else if (text == "red") return red_color;
          else if (text == "black") return black_color;
          else {
            throw bad_lexical_cast();
          }
        } else {
          throw bad_lexical_cast();
        }
      } catch (bad_lexical_cast) {
        throw cannot_parse_property(raw_text, "color");
      }
    }

    // string -> object
    inline boost::python::object
    py_convert(const boost::python::str& from, type<boost::python::object>)
    {
      return from;
    }

    // Catch-all: there is no conversion
    template<typename FromT, typename ToT>
    inline ToT
    py_convert(const FromT&, type<ToT>)
    {
      throw invalid_property_conversion("", "");
    }

    template<typename FromT, typename PropertyTag, typename ToT>
    void 
    convert_property_map(const vector_property_map<FromT, PropertyTag>& from,
                         const char* from_type,
                         vector_property_map<ToT, PropertyTag>& to,
                         const char* to_type)
    {
      typedef typename std::vector<FromT>::const_iterator from_iterator;
      typedef typename std::vector<ToT>::iterator         to_iterator;
      
      try {
        from_iterator first = from.storage_begin(), last = from.storage_end();
        for (to_iterator out = to.storage_begin(); first != last; 
             ++first, ++out)
          *out = py_convert(*first, type<ToT>());
        } catch (...) {
          throw invalid_property_conversion(from_type, to_type);
        }
    }

    template<typename PropertyTag, typename Graph>
    class python_property_map_base
    {
      typedef typename property_map<Graph, PropertyTag>::const_type IndexMap;
      typedef typename property_traits<IndexMap>::key_type Key;
      typedef python_property_map_base self_type;

    public:
      virtual ~python_property_map_base() { }
      virtual boost::python::object get(const Key& key) = 0;
      virtual void put(const Key& key, const boost::python::object& value) = 0;
      virtual int len() = 0;
      virtual boost::python::object iter() = 0;
      virtual const char* type() const = 0;
      virtual integer_type_for_addresses address() const = 0;
      virtual void* extract_as(const char*, shared_ptr<self_type>&) = 0;
      virtual shared_ptr<python_property_map_base> astype(const char*) = 0;
    };

    // Extract the number of vertices from the graph
    template<typename Graph>
    inline typename graph_traits<Graph>::vertices_size_type
    get_num_by_tag(vertex_index_t, const Graph& g) {
      return num_vertices(g);
    }

    // Extract the number of edges from the graph
    template<typename Graph>
    inline typename graph_traits<Graph>::edges_size_type
    get_num_by_tag(edge_index_t, const Graph& g) {
      return num_edges(g);
    }

    // Register a vector property map for vertices with the graph
    template<typename T, typename IndexMap, typename Graph>
    inline void 
    register_pmap_by_tag(vertex_index_t, 
                         const vector_property_map<T, IndexMap>& pmap, 
                         Graph& g)
    {
      typedef resizable_weak_vector_property_map<T, IndexMap> resize_map_type;
      std::auto_ptr<resizable_property_map> reg(new resize_map_type(pmap));
      g.register_vertex_map(reg);
    }

    // Register a vector property map for edges with the graph
    template<typename T, typename IndexMap, typename Graph>
    inline void 
    register_pmap_by_tag(edge_index_t, 
                         const vector_property_map<T, IndexMap>& pmap, 
                         Graph& g)
    {
      typedef resizable_weak_vector_property_map<T, IndexMap> resize_map_type;
      std::auto_ptr<resizable_property_map> reg(new resize_map_type(pmap));
      g.register_edge_map(reg);
    }

    // Retrieve either a value or a pointer from a reference into a
    // property map. When the type is a class type (e.g., point2d and
    // point3d), we need to return a pointer.
    template<typename T>
    inline const T& py_maybe_get_pointer(const T& value) { return value; }

    template<typename T>
    inline T& py_maybe_get_pointer(T& value) { return value; }

    inline reference_wrapper<point2d> py_maybe_get_pointer(point2d& value) { 
      return ref(value);
    }
    inline reference_wrapper<point3d> py_maybe_get_pointer(point3d& value) { 
      return ref(value); 
    }

    template<typename Value, typename PropertyTag, typename Graph>
    class python_vector_property_map 
      : public python_property_map_base<PropertyTag, Graph>
    {
      typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
      typedef typename graph_traits<Graph>::edge_descriptor   edge_descriptor;
      typedef python_property_map_base<PropertyTag, Graph> inherited;
      typedef typename property_map<Graph, PropertyTag>::const_type IndexMap;
      typedef typename property_traits<IndexMap>::key_type  key_type;

    public:
      python_vector_property_map(Graph* graph) : graph(graph)
      { 
        using boost::graph::python::get;

        // Create the property map
        pmap = 
          vector_property_map<Value, IndexMap>(get_num_by_tag(PropertyTag(),
                                                              *graph),
                                               get(PropertyTag(), *graph));

        // Register property map with graph
        register_pmap_by_tag(PropertyTag(), pmap, *graph);
      }

      virtual boost::python::object get(const key_type& key)
      {
        using boost::get;
        return boost::python::object(py_maybe_get_pointer(get(pmap, key)));
      }

      virtual void 
      put(const key_type& key, const boost::python::object& value)
      {
        using boost::put;
        put(pmap, key, boost::python::extract<Value>(value)());
      }
        
      virtual int len() 
      { 
        return pmap.get_store()->size(); 
      }

      virtual boost::python::object iter() 
      { 
        using boost::python::object;

        typedef typename std::vector<Value>::iterator value_iterator;
        typedef simple_python_iterator<value_iterator> result_type;
        result_type::declare("ValueIterator");
        return object(result_type(std::make_pair(pmap.storage_begin(), 
                                                 pmap.storage_end())));
      }

      virtual const char* type() const
      {
        typedef property_value_type_name<Value, 
                                         vertex_descriptor,
                                         edge_descriptor> Name;
        return Name::name();
      }

      virtual integer_type_for_addresses address() const
      {
        const void* addr = &*pmap.storage_begin();
        return reinterpret_cast<integer_type_for_addresses>(addr);
      }

      virtual void* extract_as(const char* type, shared_ptr<inherited>& ptr)
      {
        const char* my_type = 
          property_value_type_name<Value, vertex_descriptor, edge_descriptor>
            ::name();

        // Trying to extract with a different type. Convert to that
        // type, then replace ptr with the converted property map,
        // destroying ourselves in the process.
        if (strcmp(my_type, type) != 0)
          {
            ptr = astype(type);
            return ptr->extract_as(type, ptr);
          }
        else
          return &pmap;
      }

      virtual shared_ptr<inherited> astype(const char* type)
      { 
        // Then, convert it in place and return the result.
#define VERTEX_PROPERTY(Name,Type,Kind)                                 \
        if (strcmp(type, #Name) == 0) {                               \
          typedef python_vector_property_map<Type, PropertyTag, Graph> \
            python_map;                                               \
                                                                      \
          shared_ptr<inherited> result(new python_map(graph));        \
                                                                      \
          vector_property_map<Type, IndexMap>& new_pmap =             \
            *(vector_property_map<Type, IndexMap>*)                   \
            result->extract_as(type, result);                         \
          convert_property_map(pmap, this->type(), new_pmap, type);   \
          return result;                                              \
        }
#define EDGE_PROPERTY(Name,Type,Kind)
#include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY
        throw bad_property_type(type);
      }

      Graph* graph;
      vector_property_map<Value, IndexMap> pmap;
    };

    template<typename PropertyTag, typename Graph>
    int 
    python_property_map_length
      (const python_property_map<PropertyTag, Graph>& pm)
    {
      return pm.len();
    }
    
    template<typename PropertyTag, typename Graph>
    boost::python::object
    python_property_map_values
      (const python_property_map<PropertyTag, Graph>& pm)
    {
      return pm.iter();
    }

    template<typename Class, typename PropertyTag, typename Graph>
    void 
    property_map_extras(Class& pm, 
                        type<python_property_map<PropertyTag, Graph> >, int)
    {
      pm.def("__len__", &detail::python_property_map_length<PropertyTag, Graph>);
      pm.def("__iter__", &detail::python_property_map_values<PropertyTag, Graph>);
    }
  } // end namespace detail

  // Copy either the vertex or edge indices into a new property map
  template<typename Graph, typename PropertyMap>
  void
  copy_vertex_or_edge_indices(vertex_index_t, const Graph& graph, 
                              PropertyMap pmap)
  {
    typename graph_traits<Graph>::vertex_iterator v;
    for (v = vertices(graph).first; v != vertices(graph).second; ++v)
      put(pmap, *v, get(vertex_index, graph, *v));
  }

  template<typename Graph, typename PropertyMap>
  void
  copy_vertex_or_edge_indices(edge_index_t, const Graph& graph, 
                              PropertyMap pmap)
  {
    typename graph_traits<Graph>::edge_iterator e;
    for (e = edges(graph).first; e != edges(graph).second; ++e)
      put(pmap, *e, get(edge_index, graph, *e));
  }

  template<typename PropertyTag, typename Graph>
  class python_property_map
  {
    typedef detail::python_property_map_base<PropertyTag, Graph> base_type;
    typedef typename property_map<Graph, PropertyTag>::const_type IndexMap;

  public:
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename graph_traits<Graph>::edge_descriptor   edge_descriptor;

    // Read Write Property Map concept
    typedef typename property_traits<IndexMap>::key_type  key_type;
    typedef boost::python::object                         value_type;
    typedef value_type                                    reference;
    typedef read_write_property_map_tag                   category;

    python_property_map(Graph* graph, const char* astype)
    {
#define VERTEX_PROPERTY(Name,Type,Kind)                                 \
      if (std::strcmp(astype, #Name) == 0) {                            \
        typedef detail::python_vector_property_map<Type, PropertyTag, Graph> stored_type; \
        stored.reset(new stored_type(graph));                           \
        return;                                                         \
      }
#define EDGE_PROPERTY(Name,Type,Kind)
#include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY

      if (std::strcmp(astype, "index") == 0) {
        typedef detail::python_vector_property_map<int, PropertyTag, Graph>
          stored_type;
        stored.reset(new stored_type(graph));
        copy_vertex_or_edge_indices(PropertyTag(), *graph, 
                                    dynamic_cast<stored_type*>(&*stored)->pmap);
        return;                              
      }
      
      throw bad_property_type(astype);
    }

    template<typename Value>
    operator vector_property_map<Value, IndexMap>&() const
    {
      typedef vector_property_map<Value, IndexMap> result_type;
      const char* type_name = 
        detail::property_value_type_name<Value, 
                                         vertex_descriptor, 
                                         edge_descriptor>::name();
      return *static_cast<result_type*>(stored->extract_as(type_name, stored));
    }

    /// Return the number of elements in the property map
    int len() const { return stored->len(); }

    /// Return a Python iterator that visits all of the values in the
    /// property map.
    boost::python::object iter() const { return stored->iter(); }

    const char* type() const
    {
      return stored->type();
    }

    integer_type_for_addresses address() const
    {
      return stored->address();
    }

    // Return a new property map containing the same values as this
    // one, converted to type.
    python_property_map astype(const char* type)
    {
      python_property_map other(*this);
      other.stored = stored->astype(type);
      return other;
    }

    mutable shared_ptr<base_type> stored;
  };

  template<typename PropertyTag, typename Graph>
  boost::python::object
  get(const python_property_map<PropertyTag, Graph>& pmap, 
      typename property_traits<
                 typename property_map<Graph, PropertyTag>::const_type
               >::key_type const & key)
  {
    return pmap.stored->get(key);
  }

  template<typename PropertyTag, typename Graph>
  void
  put(const python_property_map<PropertyTag, Graph>& pmap, 
      typename property_traits<
                 typename property_map<Graph, PropertyTag>::const_type
               >::key_type const & key,
      const boost::python::object& value)
  {
    return pmap.stored->put(key, value);
  }

} } } // end namespace boost::graph::python

#endif // BOOST_GRAPH_PYTHON_PYTHON_PROPERTY_MAP_HPP
