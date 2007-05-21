// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_PARALLEL_GRAPH_PYTHON_PROPERTY_MAP_HPP
#define BOOST_PARALLEL_GRAPH_PYTHON_PROPERTY_MAP_HPP

#include <memory>
#include <boost/python.hpp>
#include <boost/graph/python/iterator.hpp>
#include <boost/property_map.hpp>
#include <boost/vector_property_map.hpp>

namespace boost { namespace graph { namespace python {

namespace detail {
  template<typename Class, typename PropertyMap>
  inline void property_map_extras(Class&, type<PropertyMap>, long) { }

  template<typename T, typename IndexMap>
  int py_vector_property_map_length(const vector_property_map<T, IndexMap>& pm)
  {
    return int(pm.get_store()->size());
  }
  
  template<typename T, typename IndexMap>
  simple_python_iterator<typename std::vector<T>::iterator>
  py_vector_property_map_values(vector_property_map<T, IndexMap>& pm)
  {
    typedef simple_python_iterator<typename std::vector<T>::iterator>
      result_type;
    return result_type(std::make_pair(pm.storage_begin(), pm.storage_end()));
  }

  template<typename Class, typename T, typename IndexMap>
  void 
  property_map_extras(Class& pm, type<vector_property_map<T, IndexMap> >, int)
  {
    pm.def("__len__", &py_vector_property_map_length<T, IndexMap>);
    
    typedef typename std::vector<T>::iterator value_iterator;
    simple_python_iterator<value_iterator>::declare("ValueIterator");
    pm.def("__iter__", &py_vector_property_map_values<T, IndexMap>);
  }

} // end namespace detail

template<typename PropertyMap>
class readable_property_map
{
  typedef typename property_traits<PropertyMap>::key_type   key_type;
  typedef typename property_traits<PropertyMap>::value_type value_type;

  static value_type getitem(const PropertyMap& pm, const key_type& key)
  { return get(pm, key); }

 public:
  template<typename T, typename Basis, typename HeldType, typename NonCopyable>
  readable_property_map(boost::python::class_<T, Basis, HeldType, NonCopyable>& pm)
  {
    pm.def("__getitem__", &getitem);
  }
};

template<typename PropertyMap>
class read_write_property_map
{
  typedef typename property_traits<PropertyMap>::key_type   key_type;
  typedef typename property_traits<PropertyMap>::value_type value_type;

  static value_type getitem(const PropertyMap& pm, const key_type& key)
  { return get(pm, key); }

  static void
  setitem(const PropertyMap& pm, const key_type& key,const value_type& value)
  { return put(pm, key, value); }

 public:
  template<typename T, typename Basis, typename HeldType, typename NonCopyable>
  read_write_property_map(boost::python::class_<T, Basis, HeldType, NonCopyable>& pm)
  {
    pm.def("__getitem__", &getitem)
      .def("__setitem__", &setitem)
      ;
    
    using detail::property_map_extras;
    property_map_extras(pm, type<PropertyMap>(), 0);
  }
};

template<typename PropertyMap>
class lvalue_property_map
{
  typedef typename property_traits<PropertyMap>::key_type   key_type;
  typedef typename property_traits<PropertyMap>::value_type value_type;
  typedef typename property_traits<PropertyMap>::reference  reference;

  static reference getitem(const PropertyMap& pm, const key_type& key)
  { return pm[key]; }

  static void
  setitem(const PropertyMap& pm, const key_type& key,const value_type& value)
  { return put(pm, key, value); }

public:
  template<typename T, typename Basis, typename HeldType, typename NonCopyable>
  lvalue_property_map(boost::python::class_<T, Basis, HeldType, NonCopyable>& pm)
  {
    using boost::python::class_;
    using boost::python::no_init;
    using boost::python::return_value_policy;
    using boost::python::return_internal_reference;

    pm.def("__getitem__", &getitem, return_internal_reference<1>())
      .def("__setitem__", &setitem)
      ;

    detail::property_map_extras(pm, type<PropertyMap>(), 0);
  }
};

} } } // end namespace boost::graph::python

#endif // BOOST_PARALLEL_GRAPH_PYTHON_PROPERTY_MAP_HPP
