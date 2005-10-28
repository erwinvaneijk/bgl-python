// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/biconnected_components.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>
#include <list>
#include <iterator>

namespace boost { namespace graph { namespace python {

using boost::python::list;

template<typename Graph>
list
biconnected_components
  (const Graph& g,
   vector_property_map<
     int, 
     typename property_map<Graph, edge_index_t>::const_type>* in_component)
{
  typedef vector_property_map<
            int, 
            typename property_map<Graph, edge_index_t>::const_type>
    ComponentMap;

  ComponentMap component = 
    in_component? *in_component 
    : ComponentMap(num_edges(g), get(edge_index, g));

  std::list<typename Graph::Vertex> art_points;
  boost::biconnected_components(g, component, std::back_inserter(art_points));
  boost::python::list result;
  for (typename std::list<typename Graph::Vertex>::iterator i 
         = art_points.begin(); i != art_points.end(); ++i)
    result.append(*i);
  return result;
}

void export_biconnected_components()
{
  using boost::python::arg;
  using boost::python::def;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef vector_property_map<int,                                    \
                                property_map<Type, edge_index_t>::const_type> \
      ComponentMap;                                                     \
                                                                        \
    def("biconnected_components", &biconnected_components<Type>,        \
        (arg("graph"),                                                  \
         arg("component_map") = static_cast<ComponentMap*>(0)));        \
  }
#define DIRECTED_GRAPH(Name,Type)
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
