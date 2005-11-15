// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/isomorphism.hpp>
#include "graph_types.hpp"
#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

template<typename Graph>
struct py_vertex_invariant
{
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

  explicit py_vertex_invariant(boost::python::object invariant)
    : invariant(invariant) { }

  int operator()(const Vertex& v, const Graph& g)
  { return boost::python::extract<int>(invariant(v, ref(g))); }

private:
  boost::python::object invariant;
};

template<typename Graph>
bool
isomorphism
  (const Graph& g1,
   const Graph& g2,
   vector_property_map<
     typename graph_traits<Graph>::vertex_descriptor,
     typename property_map<Graph, vertex_index_t>::const_type>* in_iso,
   boost::python::object invariant)
{
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef vector_property_map<vertex_descriptor, VertexIndexMap> IsoMap;

  IsoMap iso = 
    in_iso? *in_iso 
    : IsoMap(num_vertices(g1), get(vertex_index, g1));

  if (invariant != boost::python::object()) 
    return boost::isomorphism
      (g1, g2, 
       isomorphism_map(iso).
       vertex_invariant(py_vertex_invariant<Graph>(invariant)));
  else
    return boost::isomorphism(g1, g2, isomorphism_map(iso));
}

void export_isomorphism()
{
  using boost::python::arg;
  using boost::python::def;
  using boost::python::object;

#define UNDIRECTED_GRAPH(Name,Type)                                     \
  {                                                                     \
    typedef graph_traits<Type>::vertex_descriptor vertex_descriptor;    \
    typedef property_map<Type, vertex_index_t>::const_type VertexIndexMap; \
    typedef vector_property_map<vertex_descriptor, VertexIndexMap> IsoMap; \
    def("isomorphism", &isomorphism<Type>,                              \
        (arg("graph1"), arg("graph2"),                                  \
         arg("isomorphism_map") = static_cast<IsoMap*>(0),              \
         arg("vertex_invariant") = object()));                          \
  }
#include "graphs.hpp"
}

} } } // end namespace boost::graph::python
