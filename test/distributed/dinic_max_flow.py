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
    capacity = g.edge_property_map('integer')
    for e in g.edges:
        capacity[e] = random.randint(1,10)
        
    # Get the first vertex in the graph
    s = g.vertex(0)
    t = g.vertex(1)
    flow = g.edge_property_map('integer')
    pbgl.dinic_max_flow(g, s, t, capacity, flow)

# Build an Erdos-Renyi graph
run(pbgl.directed_erdos_renyi_graph(100, 0.02))
