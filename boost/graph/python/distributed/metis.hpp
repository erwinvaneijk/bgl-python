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
#include <boost/graph/metis.hpp>
#include <fstream>
#include <string>

namespace boost { namespace graph { namespace python { namespace distributed {

template<typename Graph> 
Graph*
read_metis_graph(const std::string& graph_file, 
                 const std::string& partition_file)
{
  std::ifstream in_graph(graph_file.c_str());
  boost::graph::metis_reader reader(in_graph);
  if (!partition_file.empty()) {
    return new Graph(reader.begin(), reader.end(), reader.num_vertices());
  } else {
    typedef typename boost::parallel::process_group_type<Graph>::type
      process_group_type;

    process_group_type pg;
    std::ifstream in_partition(partition_file.c_str());
    boost::graph::metis_distribution dist(in_partition, process_id(pg));
    return new Graph(reader.begin(), reader.end(), reader.num_vertices(),
                     pg, dist);
  }
}

} } } } // end namespace boost::graph::python::distributed

#endif // BOOST_GRAPH_PYTHON_METIS_HPP
