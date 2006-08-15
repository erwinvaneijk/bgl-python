# Copyright (C) 2006 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine
import boost.graph as bgl
g1 = bgl.read_graphml('../example/data/simple.graphml')
print g1.num_vertices()
g1.write_graphml('simple-clone.graphml')
g2 = bgl.read_graphml('../example/data/attributes.graphml')
g2.write_graphml('attributes-clone.graphml')
