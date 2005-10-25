# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

class bfs_visitor:
    def initialize_vertex(self, vertex, graph): pass
    def discover_vertex(self, vertex, graph): pass
    def examine_vertex(self, vertex, graph): pass
    def examine_edge(self, edge, graph): pass
    def tree_edge(self, edge, graph): pass
    def non_tree_edge(self, edge, graph): pass
    def gray_target(self, edge, graph): pass
    def black_target(self, edge, graph): pass
    def finish_vertex(self, vertex, graph): pass
