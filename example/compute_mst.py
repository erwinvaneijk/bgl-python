# Copyright 2005 The Trustees of Indiana University.

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

# Compute the minimum spanning tree of the graph
mst_edges = bgl.kruskal_minimum_spanning_tree(graph, weight)

# Compute the weight of the minimum spanning tree 
print 'MST weight =',sum([weight[e] for e in mst_edges])

# Put the weights into the label. Make MST edges solid while all other
# edges remain dashed.
graph.edge_properties['label'] = graph.edge_property_map('string')
graph.edge_properties['style'] = graph.edge_property_map('string')
for e in graph.edges:
    e.label = str(e.weight)
    if e in mst_edges:
        e.style = 'solid'
    else:
        e.style = 'dashed'

# Write out the graph in GraphViz DOT format
graph.write_graphviz('mst-out.dot')
