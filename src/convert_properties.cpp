// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

// DPG: Everything about this process seems naive to me. We
// brute-force conversions, patching up wherever we think we might run
// into trouble, and throughout this mess we make the user do more
// work than should be necessary. ARGH!!!

#include "graph_types.hpp"
#include <boost/python.hpp>
#include <boost/graph/python/property_map.hpp>
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/point3d.hpp>
#include <boost/graph/python/iterator.hpp> // for type_already_registered
#include <string>
#include <boost/graph/python/resizable_property_map.hpp>
#include <boost/lexical_cast.hpp>

namespace boost {

inline std::ostream& operator<<(std::ostream& out, default_color_type c)
{
  switch (c) {
  case white_color: return out << "white";
  case gray_color: return out << "gray";
  case green_color: return out << "green";
  case red_color: return out << "red";
  case black_color: return out << "black";
  }
  return out;
}

inline std::istream& operator>>(std::istream& in, default_color_type& c)
{
  std::string text;
  if (in >> text) {
    if (text == "white") c = white_color;
    else if (text == "gray") c = gray_color;
    else if (text == "green") c = green_color;
    else if (text == "red") c = red_color;
    else if (text == "black") c = black_color;
    else {
      in.setstate(std::ios_base::failbit);
      return in;
    }
  }
  return in;
}

} // end namespace boost

namespace boost { namespace python { 

inline std::ostream& operator<<(std::stringstream& out, boost::python::str s)
{
  using boost::python::extract;
  return (std::ostream&)out << (const char*)extract<const char*>(s);
}

} } // end namespace boost::python

namespace boost { namespace graph { namespace python {

using boost::python::object;
using boost::python::str;

template<typename T>
str to_string(const T& x)
{
  std::ostringstream out;
  out << x;
  return str(out.str().c_str());
}

template<typename T>
object to_object(const T& x)
{
  return object(x);
}

template<typename T> const T& identity(const T& x) { return x; }


template<typename T>
T from_object(object x)
{
  return boost::python::extract<T>(x)();
}

str object_to_string(object x)
{
  return str(x);
}

template<typename Target, typename Graph, typename Source, typename IndexMap,
         typename Converter>
object 
convert_property_map_impl(Graph& g, 
                          const vector_property_map<Source, IndexMap>& source,
                          Converter converter)
{
  typedef vector_property_map<Target, IndexMap> target_map_type;

  // Build the targeting property map
  target_map_type target(source.get_store()->size(), source.get_index_map());

  // Register the targeting property map with the graph
  typedef resizable_vector_property_map<Target, IndexMap> resize_pmap_type;
  std::auto_ptr<resizable_property_map> reg(new resize_pmap_type(target));
  g.register_vertex_map(reg);

  // Convert all of the values with lexical_cast.
  typename std::vector<Source>::const_iterator first = source.storage_begin();
  typename std::vector<Target>::iterator out = target.storage_begin();
  while (first != source.storage_end())
    *out++ = converter(*first++);

  return object(target);
}

template<typename Graph, typename T, typename IndexMap>
object 
convert_property_map(Graph& g, 
                     const vector_property_map<T, IndexMap>& in,
                     const std::string& type)
{
  // Cannot convert to these types
  if (type == "index" 
      || type == "vertex" 
      || type == "edge"
      || type == "point2d"
      || type == "point3d")
    return object();

  if (type == "integer")
    return convert_property_map_impl<int>(g, in, &boost::lexical_cast<int, T>);
  else if (type == "float")
    return convert_property_map_impl<float>(g, in, 
                                            &boost::lexical_cast<float, T>);
  else if (type == "string")
    return convert_property_map_impl<str>(g, in, &to_string<T>);
  else if (type == "object")
    return convert_property_map_impl<object>(g, in, &to_object<T>);
  else if (type == "color")
    return convert_property_map_impl<default_color_type>
             (g, in, &boost::lexical_cast<default_color_type, T>);
  else
    return object();
}

template<typename Graph, typename IndexMap>
object 
convert_object_property_map(Graph& g, 
                            const vector_property_map<object, IndexMap>& in,
                            const std::string& type)
{
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  typedef typename graph_traits<Graph>::edge_descriptor   Edge;

  // Cannot convert to these types
  if (type == "index") 
    return object();

  if (type == "integer")
    return convert_property_map_impl<int>(g, in, &from_object<int>);
  else if (type == "float")
    return convert_property_map_impl<float>(g, in, &from_object<float>);
  else if (type == "vertex")
    return convert_property_map_impl<Vertex>(g, in, &from_object<Vertex>);
  else if (type == "edge")
    return convert_property_map_impl<Edge>(g, in, &from_object<Edge>);
  else if (type == "string")
    return convert_property_map_impl<str>(g, in, &object_to_string);
  else if (type == "point2d")
    return convert_property_map_impl<point2d>(g, in, &from_object<point2d>);
  else if (type == "point3d")
    return convert_property_map_impl<point3d>(g, in, &from_object<point3d>);
  else if (type == "object")
    return convert_property_map_impl<object>(g, in, &identity<object>);
  else if (type == "color")
    return convert_property_map_impl<default_color_type>
             (g, in, &from_object<default_color_type>);
  else
    return object();
}

static const char* convert_property_map_docs = 
  "convert_property_map(self, source_map, type) -> object\n\n"
  "Converts the property map source_map into another property map\n"
  "with a different element type and returns that new property map.\n"
  "The old property map is unchanged. If a conversion is not supported,\n"
  "this routine will return None.\n\n"
  "Parameters:\n"
  "  source_map  A property map for the vertices or edges of the\n"
  "              given graph.\n\n"
  "  type        The element type of the resulting property map.\n"
  "              This may be any type supported by vertex_property_map\n"
  "              or edge_property_map.\n"
  ;

template<typename Graph>
void export_property_map_conversions(boost::python::class_<Graph>& graph)
{
  using boost::python::arg;
  
  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef typename property_map<Graph, edge_index_t>::const_type
    EdgeIndexMap;

  // Vertex property map conversions
  graph.def("convert_property_map", 
            &convert_property_map<Graph, int, VertexIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")),
            convert_property_map_docs);
  graph.def("convert_property_map", 
            &convert_property_map<Graph, float, VertexIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, boost::python::str, VertexIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, point2d, VertexIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, point3d, VertexIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_object_property_map<Graph, VertexIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, default_color_type, VertexIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));

  // Edge property map conversions
  graph.def("convert_property_map", 
            &convert_property_map<Graph, int, EdgeIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, float, EdgeIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, boost::python::str, EdgeIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, point2d, EdgeIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, point3d, EdgeIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_object_property_map<Graph, EdgeIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
  graph.def("convert_property_map", 
            &convert_property_map<Graph, default_color_type, EdgeIndexMap>,
            (arg("graph"), arg("source_map"), arg("type")));
}

// Explicit instantiations for the graph types we're interested in
#define UNDIRECTED_GRAPH(Name,Type)                                     \
  template void                                                         \
    export_property_map_conversions< Type >(boost::python::class_< Type >&);
#include "graphs.hpp"

} } } // end namespace boost::graph::python
