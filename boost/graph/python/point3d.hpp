// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_POINT3D_HPP
#define BOOST_GRAPH_PYTHON_POINT3D_HPP

#include <iostream>
#include <boost/graph/point_traits.hpp>

namespace boost { namespace graph { namespace python {
  class point3d
  {
  public:
    point3d() { coordinates[0] = coordinates[1] = coordinates[2] = 0.0f; }

    point3d(float x, float y, float z) 
    {
      coordinates[0] = x;
      coordinates[1] = y;
      coordinates[2] = z;
    }

    float&       operator[](std::size_t i)       { return coordinates[i]; }
    const float& operator[](std::size_t i) const { return coordinates[i]; }

  private:
    float coordinates[3];
  };

  inline std::ostream& operator<<(std::ostream& out, point3d p)
  { 
    return out << p[0] << ' ' << p[1] << ' ' << p[2];
  }

  inline std::istream& operator>>(std::istream& in, point3d& p)
  { 
    return in >> p[0] >> p[1] >> p[2]; 
  }

  inline bool operator==(const point3d& p1, const point3d& p2)
  {
    return p1[0] == p2[0] && p1[1] == p2[1] && p1[2] == p2[2];
  }

  inline bool operator!=(const point3d& p1, const point3d& p2)
  {
    return p1[0] != p2[0] || p1[1] != p2[1] || p1[2];
  }
} // end namespace python

template<>
struct point_traits<boost::graph::python::point3d>
{
  typedef float component_type;

  static std::size_t dimensions(const boost::graph::python::point3d&)
  { return 3; }
};

} } // end namespace boost::graph

#endif // BOOST_GRAPH_PYTHON_POINT3D_HPP
