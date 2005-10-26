# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine
from _distributed import *
from boost.graph import bfs_visitor, dijkstra_visitor

# Try to initialize MPI. If initialization completed and was needed,
# make sure we MPI_Finalize at exit.
import sys
if mpi_init(sys.argv):
    import atexit
    atexit.register(mpi_finalize)
