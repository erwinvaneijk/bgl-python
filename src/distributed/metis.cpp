// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/graph/python/distributed/metis.hpp>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

template<typename Graph>
void export_metis(const std::string& directedness)
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::manage_new_object;
  using boost::python::return_value_policy;

  def(("read_" + directedness + "_metis_graph").c_str(),
      &boost::graph::python::distributed::read_metis_graph<Graph>,
      return_value_policy<manage_new_object>(),
      (arg("graph_file"), arg("partition_file") = std::string()));
}

#define UNDIRECTED_GRAPH(Name,Type) \
  template void export_metis<Type>(const std::string&);
#include "graphs.hpp"

} } } } // end namespace boost::graph::distributed::python
