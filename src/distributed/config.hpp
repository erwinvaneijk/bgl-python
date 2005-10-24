// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

// This is a configuration file for the Python bindings to the
// Parallel BGL. Any settings that would usually be reserved for users
// (e.g., which process group to use) but should be fixed for a Python
// module will be contained here.

#include <boost/parallel/mpi/bsp_process_group.hpp>

namespace boost { namespace graph { namespace distributed {namespace python {

typedef boost::parallel::mpi::bsp_process_group process_group_type;

} } } } // end namespace boost::graph::distributed::python
