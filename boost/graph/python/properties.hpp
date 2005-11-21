// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
//
// Enumerates the properties that should be attached to the vertices
// and edges of the graph. This file contains only expansions of the
// macros VERTEX_PROPERTY and EDGE_PROPERTY, with the following
// signatures:
// 
//   VERTEX_PROPERTY(Name,Type,Kind)
//   EDGE_PROPERTY(Name,Type,Kind)
//
//   - Name is the name of the property type in Python, e.g., "string"
//     or "float".
//   - Type is the C++ type of the property, e.g., "point2d" or
//     "float". The types "vertex_descriptor" and "edge_descriptor"
//     are used for vertices and edges, respectively.
//   - Kind is the kind of property map, e.g., "read_write" or
//     "lvalue".

VERTEX_PROPERTY(integer, int, read_write)
VERTEX_PROPERTY(float, float, read_write)
VERTEX_PROPERTY(vertex, vertex_descriptor, read_write)
VERTEX_PROPERTY(edge, edge_descriptor, read_write)
VERTEX_PROPERTY(string, boost::python::str, read_write)
VERTEX_PROPERTY(point2d, boost::graph::python::point2d, lvalue)
VERTEX_PROPERTY(point3d, boost::graph::python::point3d, lvalue)
VERTEX_PROPERTY(object, boost::python::object, read_write)
VERTEX_PROPERTY(color, boost::default_color_type, read_write)
EDGE_PROPERTY(integer, int, read_write)
EDGE_PROPERTY(float, float, read_write)
EDGE_PROPERTY(vertex, vertex_descriptor, read_write)
EDGE_PROPERTY(edge, edge_descriptor, read_write)
EDGE_PROPERTY(string, boost::python::str, read_write)
EDGE_PROPERTY(point2d, boost::graph::python::point2d, lvalue)
EDGE_PROPERTY(point3d, boost::graph::python::point3d, lvalue)
EDGE_PROPERTY(object, boost::python::object, read_write)
EDGE_PROPERTY(color, boost::default_color_type, read_write)
