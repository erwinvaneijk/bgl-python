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

def run(g):
    # Initialize edge weights randomly
    import random
    weight = g.edge_property_map('float')
    for e in g.edges:
        weight[e] = random.uniform(0.,1.)
        
    # Get the first vertex in the graph
    s = g.vertex(0)

    # Run a simple Dijkstra's algorithm
    pbgl.dijkstra_shortest_paths(g, s, weight_map = weight)

    # Run a simple Dijkstra's shortest paths with an empty visitor
    pbgl.dijkstra_shortest_paths(g, s, weight_map = weight, \
                                 visitor = pbgl.dijkstra_visitor())

    # Run a Dijkstra's shortest paths with a tree-printing visitor
    predecessor = g.vertex_property_map('vertex')
    distance = g.vertex_property_map('float')

    class vis(pbgl.dijkstra_visitor):
        def __init__(self):
            self.edges = list()
            
        def tree_edge(self, e, g):
            self.edges.append(e)

    pbgl.dijkstra_shortest_paths(g, s, predecessor, distance, weight, vis())

# Build an Erdos-Renyi graph
run(pbgl.undirected_erdos_renyi_graph(100, 0.02))
run(pbgl.directed_erdos_renyi_graph(100, 0.02))
