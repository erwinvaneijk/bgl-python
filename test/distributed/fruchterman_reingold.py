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
    # Run FR layout
    position = g.vertex_property_map('point2d')
    pbgl.fruchterman_reingold_force_directed_layout(g, position)

# Build an Erdos-Renyi graph
run(pbgl.undirected_erdos_renyi_graph(100, 0.02))
run(pbgl.directed_erdos_renyi_graph(100, 0.02))


