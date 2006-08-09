# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine
import boost.graph as bgl

# Load a graph from the GraphViz file 'mst.dot'
graph = bgl.read_graphviz('mst.dot')

# Compute the minimum spanning tree of the graph
mst_edges = bgl.kruskal_minimum_spanning_tree(graph, graph.edge_properties['weight'])

# Compute the weight of the minimum spanning tree 
print 'MST weight =',sum([e.weight for e in mst_edges])

# Put the weights into the label. Make MST edges solid while all other
# edges remain dashed.
for e in graph.edges:
    e.label = e.weight
    if e in mst_edges:
        e.style = 'solid'
    else:
        e.style = 'dashed'

# Write out the graph in GraphViz DOT format
graph.write_graphviz('mst-out.dot')
