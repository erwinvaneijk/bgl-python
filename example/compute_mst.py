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
in_weight = graph.edge_properties['weight']
weight = graph.edge_property_map('float')
for e in graph.edges:
    weight[e] = float(in_weight[e])

# Compute the minimum spanning tree of the graph
mst_edges = bgl.kruskal_minimum_spanning_tree(graph, weight)

# Compute the weight of the minimum spanning tree 
print 'MST weight =',sum([weight[e] for e in mst_edges])

# Put the weights into the label. Make MST edges solid while all other
# edges remain dashed.
label = graph.edge_property_map('string')
style = graph.edge_property_map('string')
for e in graph.edges:
    label[e] = str(weight[e])
    if e in mst_edges:
        style[e] = 'solid'
    else:
        style[e] = 'dashed'

# Associate the label and style property maps with the graph for output
graph.edge_properties['label'] = label
graph.edge_properties['style'] = style

# Write out the graph in GraphViz DOT format
graph.write_graphviz('mst-out.dot')
