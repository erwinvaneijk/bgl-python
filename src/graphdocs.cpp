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
"__init__(self, edges = list()) -> GRAPH\n\n"
"Constructs a new graph from a list of edges. The edges argument\n"
"should be a sequence of tuples, where each tuple contains the source and\n"
"target vertices of an edge. The sources and targets can be any type,\n"
"so long as that type can be ordered with < and compared with ==, e.g.,\n"
"strings, integers, or floating-point-numbers."
  ;

const char* vertex_property_map_doc =
"vertex_property_map(self, type) -> VertexPropertyMap\n\n"
"Creates a new property map that maps from vertices in the graph to\n"
"values of the given type. The type parameter may be any one of:\n"
"   integer\n"
"   float\n"
"   vertex\n"
"   edge\n"
"   string\n"
"   point2d\n"
"   point3d\n"
"   object\n"
"   color\n"
  ;

const char* edge_property_map_doc =
"edge_property_map(self, type) -> EdgePropertyMap\n\n"
"Creates a new property map that maps from edges in the graph to\n"
"values of the given type. The type parameter may be any one of:\n"
"   integer\n"
"   float\n"
"   vertex\n"
"   edge\n"
"   string\n"
"   point2d\n"
"   point3d\n"
"   object\n"
"   color\n"
  ;

} } } // end namespace boost::graph::python
