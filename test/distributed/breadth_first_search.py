# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

# So we can find the BGL Python bindings
import sys
sys.path.append("../../python")

import boost.graph.distributed as pbgl

print "Building Erdos-Renyi graph..."
g = pbgl.undirected_erdos_renyi_graph(10, 0.1)

# Get the first vertex in the graph
s = g.vertex(0)

# Run a simple BFS
print "Simple Breadth-First Search..."
pbgl.breadth_first_search(g, s)

# Run a simple BFS with an empty visitor
print "Simple Breadth-First Search with empty visitor..."
pbgl.breadth_first_search(g, s, visitor = pbgl.bfs_visitor())

# Run a BFS with a tree-printing visitor
print "Breadth-First Search with tree-printing visitor..."
class vis(pbgl.bfs_visitor):
    def tree_edge(self, e, g):
        print g.source(e),"->",g.target(e)
        
pbgl.breadth_first_search(g, s, visitor = vis())
