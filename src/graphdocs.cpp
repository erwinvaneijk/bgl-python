// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/graph/python/graph.hpp>

namespace boost { namespace graph { namespace python {

const char* graph_docs[gd_last] = {
  "An iterator that enumerates the vertices in a graph",
  "num_vertices(self) -> int\n\nReturn the number of vertices in the graph.",
  "vertices(self) -> VertexIterator\n\nEnumerate the vertices in the graph.",
  "An iterator that enumerates the edges in a graph",
  "num_edges(self) -> int\n\nReturn the number of edges in the graph.",
  "edges(self) -> VertexIterator\n\nEnumerate the edges in the graph.",
  "An iterator that enumerates edges outgoing from a vertex.",
  "source(self, e) -> Vertex\n\nReturns the source of edge e.",
  "target(self, e) -> Vertex\n\nReturns the target of edge e.",
  "out_degree(self, u) -> int\n\nReturns the number of outgoing edges from u.",
  "out_edges(self, u) ->OutEdgeIterator\n\nEnumerate edges outgoing from u.",
  "An iterator that enumerates edges incoming to a vertex.",
  "in_degree(self, u) -> int\n\nReturns the number of incoming edges to u.",
  "in_edges(self, u) ->InEdgeIterator\n\nEnumerate edges incoming to u.",
  "An iterator over the vertices adjacent to a given vertex.",
  "adjacent_vertices(self, u) -> AdjacencyIterator\n\n" \
  "Enumerate vertices adjacent to u.",
  "add_edge(self, u, v) -> Edge\n\nAdd an edge between two vertices.\n" \
  "Self-loops and parallel edges are permitted.",
  "remove_edge(self, e)\n\nRemoves edge e from the graph.",
  "add_vertex(self) -> Vertex\n\nAdds a new vertex to the graph.",
  "clear_vertex(self, v)\n\nRemoves all outgoing and incoming edges from v.",
  "remove_vertex(self, v)\n\nRemoves a vertex from the graph.\n" \
  "Vertex v must have no incoming or outgoing edges. If you aren't sure,\n" \
  "call clear_vertex first."
};

const char* graph_init_doc = 
"__init__(self, edges = list(), name_map = 'node_id') -> GRAPH\n\n"
"Constructs a new graph from a list of edges. The edges argument\n"
"should be a sequence of tuples, where each tuple contains the source and\n"
"target vertices of an edge. The sources and targets can be any type,\n"
"so long as that type can be ordered with < and compared with ==, e.g.,\n"
"strings, integers, or floating-point-numbers. The name_map argument is the\n"
"name of the vertex property map that will be used to store the mapping from\n"
"vertices to their names. After this constructor completes,\n"
"  self.vertex_properties[name_map][v]\n"
"will contain the name of vertex v as it appears in the edge list."
  ;

const char* add_vertex_property_doc =
"add_vertex_property(self, name='', type='object') -> GRAPHVertexPropertyMap\n\n"
"Creates a new property map that maps from vertices in the graph to\n"
"values of the given type. If a name is given, the new property will be\n"
"added to self.vertex_properties. Otherwise, a new, unnamed property will be\n"
"added to the vertices in the graph. The type parameter may be any string\n"
"from the following table:\n\n"
"   Name  \tC++ type\n"
"--------------------\n"
#define VERTEX_PROPERTY(Name,Type,Kind)         \
"   " #Name " \t" #Type "\n"
#define EDGE_PROPERTY(Name,Type,Kind)
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY
"   index \tint (contains index of each vertex)\n"
  ;

const char* add_edge_property_doc =
"add_edge_property(self, name='', type='object') -> GRAPHEdgePropertyMap\n\n"
"Creates a new property map that maps from edges in the graph to\n"
"values of the given type. If a name is given, the new property will be\n"
"added to self.edge_properties. Otherwise, a new, unnamed property will be\n"
"added to the edges in the graph. The type parameter may be any string\n"
"from the following table:\n\n"
"   Name  \tC++ type\n"
"--------------------\n"
#define VERTEX_PROPERTY(Name,Type,Kind)
#define EDGE_PROPERTY(Name,Type,Kind)           \
"   " #Name " \t" #Type "\n"
#  include <boost/graph/python/properties.hpp>
#undef EDGE_PROPERTY
#undef VERTEX_PROPERTY
"   index \tint (contains index of each edge)\n"
  ;

} } } // end namespace boost::graph::python
