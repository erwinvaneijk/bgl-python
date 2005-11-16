# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

class bellman_ford_visitor:
    """
    Default visitor for the Bellman-Ford shortest paths algorithm.

    See also:
      bellman_ford_shortest_paths
      
    Complete C++ documentation is available at:
      http://www.boost.org/libs/graph/doc/BellmanFordVisitor.html
    """
    def initialize_vertex(self, vertex, graph):
        """
        Invoked on each vertex in the graph when the algorithm initializes it.
        """
        pass
    def examine_edge(self, edge, graph):
        """
        Invoked on each edge when the algorithm examines it.
        """
        pass
    def edge_relaxed(self, edge, graph):
        """
        Invoked on an edge when the edge is relaxed.
        """
        pass
    def edge_not_relaxed(self, edge, graph):
        """
        Invoked on an edge when the edge is not relaxed.
        """
        pass
    def edge_minimized(self, edge, graph):
        """
        Invoked when an edge is determined to have been minimized by
        the algorithm.
        """
        pass
    def edge_not_minimized(self, edge, graph):
        """
        Invoked when an edge is determined to be part of a negative
        cycle, because it has not been minimized correctly.
        """
        pass
    
