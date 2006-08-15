// Copyright (C) 2006 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "exception.hpp"
#include <boost/graph/python/python_property_map.hpp>

namespace boost { namespace graph { namespace python {

void export_exceptions()
{
  using boost::graph::python::bad_property_type;
  using boost::graph::python::invalid_property_conversion;
  using boost::graph::python::cannot_parse_property;
  using boost::python::class_;
  using boost::python::object;
  using boost::python::no_init;

  object bad_property_type_type = 
    class_<bad_property_type>("BadPropertyMap", no_init)
      .def("__str__", &bad_property_type::what);

  translate_exception<bad_property_type>::declare(bad_property_type_type);


  object invalid_property_conversion_type = 
    class_<invalid_property_conversion>("InvalidPropertyConversion", no_init)
      .def("__str__", &invalid_property_conversion::what);

  translate_exception<invalid_property_conversion>::declare(invalid_property_conversion_type);

  object cannot_parse_property_type = 
    class_<cannot_parse_property>("CannotParseProperty", no_init)
      .def("__str__", &cannot_parse_property::what);

  translate_exception<cannot_parse_property>::declare(cannot_parse_property_type);

}

} } } // end namespace boost::graph::python
