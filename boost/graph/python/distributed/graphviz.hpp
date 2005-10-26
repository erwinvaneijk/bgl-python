// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_METIS_HPP
#define BOOST_GRAPH_PYTHON_METIS_HPP

#include <memory>
#include <boost/python.hpp>
#include <boost/parallel/container_traits.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/python/graph.hpp>
#include <boost/graph/distributed/graphviz.hpp>
#include <iostream>
#include <string>

namespace boost { namespace graph { namespace python { namespace distributed {

using boost::python::object;

class python_graphviz_writer
{
 public:
  python_graphviz_writer(object callable) : callable(callable) { }

  void operator()(std::ostream& out)
  {
    using boost::python::extract;

    if (callable != object())
      out << std::string(extract<std::string>(callable()));
  }

  template<typename Descriptor>
  void operator()(std::ostream& out, Descriptor x)
  {
    using boost::python::extract;

    if (callable != object())
      out << std::string(extract<std::string>(callable(x)));
  }

 private:
  object callable;
};

template<typename Graph>
void 
write_graphviz(const Graph& g, const std::string& filename,
               object vpw, object epw, object gpw)
{
  boost::write_graphviz_distributed(filename.c_str(), g, 
                                    python_graphviz_writer(vpw),
                                    python_graphviz_writer(epw),
                                    python_graphviz_writer(gpw));
}

} } } } // end namespace boost::graph::python::distributed

#endif // BOOST_GRAPH_PYTHON_METIS_HPP
