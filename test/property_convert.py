# Copyright (C) 2006 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

# Test property conversions

import boost.graph as bgl
g = bgl.Graph([(1, 2), (1, 3), (4, 6), (5, 6)])

color = g.add_vertex_property('color')

bgl.connected_components(g, color)

for v in g.vertices:
    print color[v]

    
