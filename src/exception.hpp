// Copyright (C) 2005, 2006 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_EXCEPTION_HPP
#define BOOST_GRAPH_PYTHON_EXCEPTION_HPP

#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

template<typename E>
class translate_exception
{
  explicit translate_exception(boost::python::object type) : type(type) { }

public:
  template<typename Base>
  static void declare(const char* name)
  {
    using boost::python::class_;
    using boost::python::bases;

    declare(class_<E, bases<Base> >(name));
  }

  static void declare(boost::python::object type)
  {
    using boost::python::register_exception_translator;
    register_exception_translator<E>(translate_exception(type));
  }

  void operator()(const E& e) const
  {
    using boost::python::object;
    PyErr_SetObject(type.ptr(), object(e).ptr());
  }

private:
  boost::python::object type;
};

} } } // end namespace boost::graph::python

#endif // BOOST_GRAPH_PYTHON_EXCEPTION_HPP
