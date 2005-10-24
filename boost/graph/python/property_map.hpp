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
#include <boost/graph/python/iterator.hpp> // for type_already_registered
#include <boost/property_map.hpp>

namespace boost { namespace graph { namespace python {

template<typename PropertyMap>
class readable_property_map
{
  typedef typename property_traits<PropertyMap>::key_type   key_type;
  typedef typename property_traits<PropertyMap>::value_type value_type;

  static value_type getitem(const PropertyMap& pm, const key_type& key)
  { return get(pm, key); }

 public:
  template<typename T, typename Basis, typename HeldType, typename NonCopyable>
  readable_property_map(class_<T, Basis, HeldType, NonCopyable>& pm)
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
  read_write_property_map(class_<T, Basis, HeldType, NonCopyable>& pm)
  {
    pm.def("__getitem__", &getitem)
      .def("__setitem__", &setitem)
      ;
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
  lvalue_property_map(class_<T, Basis, HeldType, NonCopyable>& pm)
  {
    using boost::python::class_;
    using boost::python::no_init;
    using boost::python::return_value_policy;
    using boost::python::return_internal_reference;

    pm.def("__getitem__", &getitem, return_internal_reference<1>())
      .def("__setitem__", &setitem)
      ;
  }
};

} } } // end namespace boost::graph::python

#endif // BOOST_PARALLEL_GRAPH_PYTHON_PROPERTY_MAP_HPP
