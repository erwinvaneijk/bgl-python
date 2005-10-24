// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/parallel/property_map.hpp>
#include <boost/graph/python/distributed/dijkstra_shortest_paths.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

void export_dijkstra_shortest_paths()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

  typedef graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef property_map<Graph, vertex_index_t>::type VertexIndexMap;
  typedef vector_property_map<vertex_descriptor, VertexIndexMap>
    VertexPredecessorMap;
  typedef vector_property_map<double, VertexIndexMap>
    VertexDistanceMap;
  typedef vector_property_map<default_color_type, VertexIndexMap>
    VertexColorMap;

  def("dijkstra_shortest_paths", 
      &boost::graph::python::distributed::dijkstra_shortest_paths<Graph>,
      (arg("graph"), 
       arg("root_vertex"),
       arg("predecessor_map") = static_cast<VertexPredecessorMap*>(0),
       arg("distance_map") = static_cast<VertexDistanceMap*>(0),
       arg("weight_map"),
       arg("visitor") = object(),
       arg("color_map") = static_cast<VertexColorMap*>(0)));
}


} } } } // end namespace boost::graph::distributed::python
