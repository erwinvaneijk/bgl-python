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

# Build an Erdos-Renyi graph
g = pbgl.undirected_erdos_renyi_graph(100, 0.02)

# Initialize edge weights randomly
import random
weight = g.edge_property_map('float')
for e in g.edges:
    weight[e] = random.uniform(0.,1.)


# Compute the MST with the four different routines
pbgl.dense_boruvka_minimum_spanning_tree(g, weight)
pbgl.merge_local_minimum_spanning_trees(g, weight)
pbgl.boruvka_then_merge(g, weight)
pbgl.boruvka_mixed_merge(g, weight)

