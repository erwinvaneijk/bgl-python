# Copyright (C) 2006 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine
import boost.graph as bgl

# Load a graph from the GraphViz file 'mst.dot'
graph = bgl.Graph.read_graphviz('mst.dot')

# Convert the weight into floating-point values
weight = graph.convert_property_map(graph.edge_properties['weight'],
                                    'float')

# Compute the shortest paths from the first vertex ('A')
A = graph.vertices.next()
distance = graph.vertex_property_map('float')
predecessor = graph.vertex_property_map('vertex')
bgl.dijkstra_shortest_paths(graph, A, distance_map = distance, 
                            predecessor_map = predecessor, 
                            weight_map = weight)

# Print out the shortest paths tree
name = graph.vertex_properties['node_id']
for v in graph.vertices:
    print name[v],'distance from A = ',distance[v],
    print ', predecessor = ', name[predecessor[v]]
