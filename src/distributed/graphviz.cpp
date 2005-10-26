// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/graph/python/distributed/graphviz.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

template<typename Graph>
void export_graphviz(const std::string&)
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

  def("write_graphviz",
      &boost::graph::python::distributed::write_graphviz<Graph>,
      (arg("graph"), arg("filename"), 
       arg("vertex_property_writer") = object(),
       arg("edge_property_writer") = object(),
       arg("graph_property_writer") = object()));
}

#define UNDIRECTED_GRAPH(Name,Type) \
  template void export_graphviz<Type>(const std::string&);
#include "graphs.hpp"

} } } } // end namespace boost::graph::distributed::python
