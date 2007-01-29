# Copyright (C) 2006 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine
import boost.graph as bgl

# Load a graph from the GraphViz file 'mst.dot'
graph = bgl.Graph.read_graphviz('mst.dot')

# Compute the shortest paths from the first vertex ('A')
A = graph.vertices.next()
bgl.bellman_ford_shortest_paths(graph, A,
                                distance_map = graph.add_vertex_property('distance') , 
                                predecessor_map = graph.add_vertex_property('predecessor'), 
                                weight_map = graph.edge_properties['weight'])

# Print out the shortest paths tree
for v in graph.vertices:
    print v.node_id,'distance from A = ',v.distance,
    print ', predecessor = ', v.predecessor.node_id
