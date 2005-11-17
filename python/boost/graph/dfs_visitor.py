# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

class dfs_visitor:
    """
    Default visitor for the depth-first search algorithm and its variants.

    See also:
      depth_first_search
      depth_first_visit
      
    Complete C++ documentation is available at:
      http://www.boost.org/libs/graph/doc/DFSVisitor.html
    """

    def initialize_vertex(self, vertex, graph): 
        """
        Invoked on each vertex in the graph when the algorithm initializes it.
        """
        pass

    def start_vertex(self, vertex, graph): 
        """
        Invoked on the starting vertex just before the search begins.
        """
        pass

    def discover_vertex(self, vertex, graph): 
        """
        Invoked on a vertex when it is first "discovered" by the algorithm.
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

    def tree_edge(self, edge, graph): 
        """
        Invoked on an edge when it is determined that it is an edge in
        the depth-first search tree.
        """
        pass

    def back_edge(self, edge, graph): 
        """
        Invoked on the back edges of the depth-first tree, i.e., edges
        back to the ancestors of a node.
        """
        pass

    def forward_or_cross_edge(self, edge, graph): 
        """ 
        Invoked on forward or cross edges in the depth-first tree,
        i.e., edges to vertices in the graph that have already been
        completely handled.
        """
        pass

    def finish_vertex(self, vertex, graph): 
        """
        Invoked on a vertex after all of its outgoing edges have been
        examined.
        """
        pass

