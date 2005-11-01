// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

#include "graph_types.hpp"
#include <boost/graph/graphviz.hpp>
#include <boost/python.hpp>
#include <fstream>
#include <string>
#include <iostream>

namespace boost { namespace graph { namespace python {

template<typename Graph>
Graph*
read_graphviz(const std::string& filename, const std::string& node_id)
{
  std::auto_ptr<Graph> result(new Graph);
  std::ifstream in(filename.c_str());
  boost::read_graphviz(in, *result, result->get_dynamic_properties(), node_id);
  return result.release();
}

// DPG TBD: Handle node_id properly
template<typename Graph>
void
write_graphviz(const Graph& g, const std::string& filename, 
               const std::string& node_id)
{
  std::ofstream out(filename.c_str());

  boost::write_graphviz(out, g, g.get_dynamic_properties(), node_id, 
                        get(vertex_index, g));
}

template<typename E>
class translate_exception
{
  explicit translate_exception(boost::python::object type) : type(type) { }

public:
  template<typename Base>
  static void declare(const char* name)
  {
    using boost::python::class_;
    using boost::python::bases;

    declare(class_<E, bases<Base> >(name));
  }

  static void declare(boost::python::object type)
  {
    using boost::python::register_exception_translator;
    register_exception_translator<E>(translate_exception(type));
  }

  void operator()(const E& e) const
  {
    using boost::python::object;
    PyErr_SetObject(type.ptr(), object(e).ptr());
  }

private:
  boost::python::object type;
};

void export_graphviz_exceptions()
{
  using boost::python::class_;
  using boost::python::bases;
  using boost::python::init;
  using boost::python::no_init;
  using boost::python::object;

  object ge_type = 
    class_<graph_exception, noncopyable >("graph_exception", no_init);
  translate_exception<graph_exception>::declare(ge_type);

  object bpe_type = 
    class_<bad_parallel_edge, bases<graph_exception> >("bad_parallel_edge", 
                                                       no_init)
      .def(init<std::string, std::string>());
  translate_exception<bad_parallel_edge>::declare(bpe_type);

  translate_exception<directed_graph_error>
    ::declare<graph_exception>("directed_graph_error");
  translate_exception<undirected_graph_error>
    ::declare<graph_exception>("undirected_graph_error");
}

template<typename Graph>
void export_graphviz(boost::python::class_<Graph>& graph)
{
  using boost::python::arg;
  using boost::python::manage_new_object;
  using boost::python::return_value_policy;

  graph.def("read_graphviz", &read_graphviz<Graph>,
            return_value_policy<manage_new_object>(),
            (arg("filename"), arg("node_id") = "node_id"))
    .staticmethod("read_graphviz");
  
  graph.def("write_graphviz", &write_graphviz<Graph>,
            (arg("graph"), arg("filename"), arg("node_id") = "node_id"));
}

// Explicit instantiations
#define UNDIRECTED_GRAPH(Name,Type)                                     \
  template void export_graphviz(boost::python::class_<Type>& graph);    \
  template                                                              \
    Type* read_graphviz(const std::string&, const std::string&);        \
  template                                                              \
    void write_graphviz(const Type&, const std::string&,  const std::string&);
#include "graphs.hpp"
} } } // end namespace boost::graph::python
