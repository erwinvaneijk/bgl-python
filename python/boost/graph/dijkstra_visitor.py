# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

class dijkstra_visitor:
    """
    Default visitor for Dijkstra's single source shortest paths
    algorithm and related algorithms.

    See also:
      dijkstra_shortest_paths
      prim_minimum_spanning_tree
      
    Complete C++ documentation is available at:
      http://www.boost.org/libs/graph/doc/DijkstraVisitor.html
    """

    def initialize_vertex(self, vertex, graph): 
        """
        Invoked on each vertex in the graph when the algorithm initializes it.
        """
        pass

    def examine_vertex(self, vertex, graph): 
        """
        Invoked on a vertex just before its outgoing edges will be
        examined.
        """
        pass

    def examine_edge(self, edge, graph): 
        """
        Invoked on an edge as it is being examined.
        """
        pass

    def discover_vertex(self, vertex, graph): 
        """
        Invoked on a vertex when it is first "discovered" by the algorithm.
        """
        pass

    def edge_relaxed(self, edge, graph): 
        """
        Invoked on an edge when the edge is relaxed, i.e., it has been
        used to find a shorter path to its target.
        """
        pass

    def edge_not_relaxed(self, edge, graph): 
        """
        Invoked on an edge when the edge is not relaxed.
        """
        pass

    def finish_vertex(self, vertex, graph): 
        """
        Invoked on a vertex after all of its outgoing edges have been
        examined.
        """
        pass
