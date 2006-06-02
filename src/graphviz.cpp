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
#include <boost/algorithm/string/replace.hpp>
#include <boost/graph/python/dynamic_properties.hpp>

namespace boost { namespace python { 

inline std::ostream& operator<<(std::ostringstream& out, boost::python::str s)
{
  using boost::python::extract;
  return (std::ostream&)out << (const char*)extract<const char*>(s);
}

} } // end namespace boost::python

namespace boost { namespace graph { namespace python {

const char* read_graphviz_doc = 
"read_graphviz(filename, node_id = 'node_id') -> GRAPH\n\n"
"Loads a graph written in GraphViz DOT format from the file filename.\n"
"Parameters:\n"
"  filename  The name of the file to load.\n"
"  node_id   The name given to the property map that will store the\n"
"            identifier associated with each vertex in the DOT file.\n\n"
"Exceptions:\n"
"  directed_graph_error    Thrown if one tries to read a directed graph\n"
"                          into the Graph class.\n"
"  undirected_graph_error  Thrown if one tries to read an undirected\n"
"                          graph into the Digraph class.\n\n"
"See also:\n"
"  write_graphviz\n\n"
"The GraphViz DOT language is described here:\n"
"  http://www.graphviz.org/doc/info/lang.html\n\n"
"Complete C++ documentation is available at:\n"
"  http://www.boost.org/libs/graph/doc/read_graphviz.html\n"
  ;

const char* write_graphviz_doc = 
"write_graphviz(self, filename)\n\n"
"Writes the graph into the file filename (overwriting the file if it \n"
"already exists) using the GraphViz DOT format.\n\n"
"See also:\n"
"  read_graphviz\n\n"
"The GraphViz DOT language is described here:\n"
"  http://www.graphviz.org/doc/info/lang.html\n\n"
"Complete C++ documentation is available at:\n"
"  http://www.boost.org/libs/graph/doc/write-graphviz.html\n"
  ;

template<typename Graph>
Graph*
read_graphviz(const std::string& filename, const std::string& node_id)
{
  std::auto_ptr<Graph> result(new Graph);
  std::ifstream in(filename.c_str());
  build_string_property_maps<Graph> builder(result.get());
  dynamic_properties dp(builder);
  boost::read_graphviz(in, *result, dp, node_id);
  string_properties_to_dicts<Graph>(*result, dp);
  return result.release();
}

template<typename Graph>
void
write_graphviz(const Graph& g, const std::string& filename, 
               const std::string& node_id)
{
  using boost::python::object;
  using boost::python::str;
  using boost::python::extract;

  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor Edge;

  std::ofstream out(filename.c_str());
  dynamic_properties dp;
  dict_to_properties<Vertex>(g.vertex_properties(), dp);
  dict_to_properties<Edge>(g.edge_properties(), dp);

  if (g.vertex_properties().has_key(str(node_id))) {
    object node_id_map = g.vertex_properties()[str(node_id)];
    boost::write_graphviz(out, g, dp, node_id, 
                          object_as_string_property_map<Vertex>(node_id_map));
  } else {
    boost::write_graphviz(out, g, dp, node_id, 
                          get(vertex_index, g));
  }
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
void export_graphviz(boost::python::class_<Graph>& graph, const char* name)
{
  using boost::python::arg;
  using boost::python::manage_new_object;
  using boost::python::return_value_policy;

  std::string my_read_graphviz_doc(read_graphviz_doc);
  algorithm::replace_all(my_read_graphviz_doc, "GRAPH", name);

  graph.def("read_graphviz", &read_graphviz<Graph>,
            return_value_policy<manage_new_object>(),
            (arg("filename"), arg("node_id") = "node_id"),
            my_read_graphviz_doc.c_str())
    .staticmethod("read_graphviz");
  
  graph.def("write_graphviz", &write_graphviz<Graph>,
            (arg("graph"), arg("filename"), arg("node_id") = "node_id"),
            write_graphviz_doc);
}

// Explicit instantiations
#define UNDIRECTED_GRAPH(Name,Type)                                     \
  template void export_graphviz(boost::python::class_<Type>& graph,     \
                                const char* name);                      \
  template                                                              \
    Type* read_graphviz(const std::string&, const std::string&);        \
  template                                                              \
    void write_graphviz(const Type&, const std::string&,  const std::string&);
#include "graphs.hpp"
} } } // end namespace boost::graph::python
