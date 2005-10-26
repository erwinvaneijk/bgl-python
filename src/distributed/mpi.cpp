// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include <boost/python.hpp>
#include <mpi.h>
#include <cassert>
#include <cstring>

namespace boost { namespace graph { namespace distributed { namespace python {

bool mpi_init(boost::python::list python_argv)
{
  using boost::python::extract;
  using boost::python::object;

  // If already initialized, do nothing but note that initialization
  // is done.
  int flag = 0;
  int result = MPI_Initialized(&flag);
  assert(result == MPI_SUCCESS);
  if (flag) return false;

  // Convert Python argv into C-style argc/argv. Ewwwww!
  int my_argc = extract<int>(python_argv.attr("__len__")());
  char** my_argv = new char*[my_argc];
  for (int arg = 0; arg < my_argc; ++arg)
    my_argv[arg] = strdup(extract<const char*>(python_argv[arg]));

  // Initialize MPI
  int mpi_argc = my_argc;
  char** mpi_argv = my_argv;
  result = MPI_Init(&mpi_argc, &mpi_argv);
  assert(result == MPI_SUCCESS);

  // If anything changed, convert C-style argc/argv into Python argv
  if (mpi_argv != my_argv) {
    // Tear down Python argv
    while (int(extract<int>(python_argv.attr("__len__")())) > 0)
      python_argv.pop();

    // Build up new Python argv
    for (int arg = 0; arg < mpi_argc; ++arg)
      python_argv.append(object(mpi_argv[arg]));
  }

  for (int arg = 0; arg < my_argc; ++arg)
    free(my_argv[arg]);
  delete [] my_argv;

  return true;
}

void mpi_finalize()
{
  int result = MPI_Finalize();
  assert(result == MPI_SUCCESS);
}

void export_mpi()
{
  using boost::python::arg;
  using boost::python::def;

  def("mpi_init", &mpi_init, (arg("argv")));
  def("mpi_finalize", &mpi_finalize);
}

} } } } // end namespace boost::graph::distributed::python
