// Copyright 2004, 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_FRUCHTERMAN_REINGOLD_FORCE_DIRECTED_LAYOUT_HPP
#define BOOST_GRAPH_FRUCHTERMAN_REINGOLD_FORCE_DIRECTED_LAYOUT_HPP

#include <cmath>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <vector>
#include <list>
#include <algorithm> // for std::min, std::max, std::accumulate
#include <boost/graph/point_traits.hpp>
#include <functional>
#include <numeric>

#include <stdlib.h> // for drand48

namespace boost {

using boost::graph::point_traits;

  bool vertex_migration = false;

struct square_distance_attractive_force {
  template<typename Graph, typename T>
  T
  operator()(typename graph_traits<Graph>::edge_descriptor,
             T k,
             T d,
             const Graph&) const
  {
    return d * d / k;
  }
};

struct square_distance_repulsive_force {
  template<typename Graph, typename T>
  T
  operator()(typename graph_traits<Graph>::vertex_descriptor,
             typename graph_traits<Graph>::vertex_descriptor,
             T k,
             T d,
             const Graph&) const
  {
    return k * k / d;
  }
};

template<typename T>
struct linear_cooling {
  typedef T result_type;

  linear_cooling(std::size_t iterations)
    : temp(T(iterations) / T(10)), step(0.1) { }

  linear_cooling(std::size_t iterations, T temp)
    : temp(temp), step(temp / T(iterations)) { }

  T operator()()
  {
    T old_temp = temp;
    temp -= step;
    if (temp < T(0)) temp = T(0);
    return old_temp;
  }

 private:
  T temp;
  T step;
};

struct all_force_pairs
{
  template<typename Graph, typename ApplyForce >
  void operator()(const Graph& g, ApplyForce apply_force)
  {
    typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
    vertex_iterator v, end;
    for (tie(v, end) = vertices(g); v != end; ++v) {
      vertex_iterator u = v;
      for (++u; u != end; ++u) {
        apply_force(*u, *v);
        apply_force(*v, *u);
      }
    }
  }
};

template<typename PositionMap>
struct grid_force_pairs
{
  typedef typename property_traits<PositionMap>::value_type Point;
  typedef typename point_traits<Point>::component_type Dim;

  template<typename Graph>
  explicit
  grid_force_pairs(const Point& origin, const Point& extent, 
                   PositionMap position, const Graph& g)
    : width(extent[0]), height(extent[1]), position(position)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    using std::sqrt;
#endif // BOOST_NO_STDC_NAMESPACE
    two_k = Dim(2) * sqrt(width * height / num_vertices(g));
  }

  template<typename Graph, typename ApplyForce >
  void operator()(const Graph& g, ApplyForce apply_force)
  {
    typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef std::list<vertex_descriptor> bucket_t;
    typedef std::vector<bucket_t> buckets_t;

#ifndef BOOST_NO_STDC_NAMESPACE
    using std::sqrt;
#endif // BOOST_NO_STDC_NAMESPACE

    std::size_t columns = std::size_t(width / two_k + Dim(1));
    std::size_t rows = std::size_t(height / two_k + Dim(1));
    buckets_t buckets(rows * columns);
    vertex_iterator v, v_end;
    for (tie(v, v_end) = vertices(g); v != v_end; ++v) {
      std::size_t column = std::size_t((position[*v][0] + width  / 2) / two_k);
      std::size_t row    = std::size_t((position[*v][1] + height / 2) / two_k);

      if (column >= columns) column = columns - 1;
      if (row >= rows) row = rows - 1;
      buckets[row * columns + column].push_back(*v);
    }

    for (std::size_t row = 0; row < rows; ++row)
      for (std::size_t column = 0; column < columns; ++column) {
        bucket_t& bucket = buckets[row * columns + column];
        typedef typename bucket_t::iterator bucket_iterator;
        for (bucket_iterator u = bucket.begin(); u != bucket.end(); ++u) {
          // Repulse vertices in this bucket
          bucket_iterator v = u;
          for (++v; v != bucket.end(); ++v) {
            apply_force(*u, *v);
            apply_force(*v, *u);
          }

          std::size_t adj_start_row = row == 0? 0 : row - 1;
          std::size_t adj_end_row = row == rows - 1? row : row + 1;
          std::size_t adj_start_column = column == 0? 0 : column - 1;
          std::size_t adj_end_column = column == columns - 1? column : column + 1;
          for (std::size_t other_row = adj_start_row; other_row <= adj_end_row;
               ++other_row)
            for (std::size_t other_column = adj_start_column; 
                 other_column <= adj_end_column; ++other_column)
              if (other_row != row || other_column != column) {
                // Repulse vertices in this bucket
                bucket_t& other_bucket 
                  = buckets[other_row * columns + other_column];
                for (v = other_bucket.begin(); v != other_bucket.end(); ++v) {
                  Dim delta_x = position[*u][0] - position[*v][0]; 
                  Dim delta_y = position[*u][1] - position[*v][1]; 
                  Dim dist = sqrt(delta_x*delta_x + delta_y*delta_y);
                  if (dist < two_k) apply_force(*u, *v);
                }
              }
        }
      }
  }

 private:
  Dim width;
  Dim height;
  PositionMap position;
  Dim two_k;
};

template<typename PositionMap, typename Graph>
inline grid_force_pairs<PositionMap>
make_grid_force_pairs
  (typename property_traits<PositionMap>::value_type const& origin,
   typename property_traits<PositionMap>::value_type const& extent,
   const PositionMap& position, const Graph& g)
{ return grid_force_pairs<PositionMap>(origin, extent, position, g); }

template<typename Graph, typename PositionMap, typename Dim>
void
scale_graph(const Graph& g, PositionMap position,
            Dim left, Dim top, Dim right, Dim bottom)
{
  if (num_vertices(g) == 0) return;

  if (bottom > top) {
    using std::swap;
    swap(bottom, top);
  }

  typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;

  // Find min/max ranges
  Dim minX = position[*vertices(g).first][0], maxX = minX;
  Dim minY = position[*vertices(g).first][1], maxY = minY;
  vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    BOOST_USING_STD_MIN();
    BOOST_USING_STD_MAX();
    minX = (min)(minX, position[*vi][0]);
    maxX = (max)(maxX, position[*vi][0]);
    minY = (min)(minY, position[*vi][1]);
    maxY = (max)(maxY, position[*vi][1]);
  }

  // Scale to bounding box provided
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    position[*vi][0] = ((position[*vi][0] - minX) / (maxX - minX))
                    * (right - left) + left;
    position[*vi][1] = ((position[*vi][1] - minY) / (maxY - minY))
                    * (top - bottom) + bottom;
  }
}

namespace detail {
  template<typename Point>
  void 
  maybe_jitter_point(Point& p1, const Point& p2, Point origin, Point extent)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    using std::sqrt;
    using std::fabs;
#endif // BOOST_NO_STDC_NAMESPACE
    typedef typename point_traits<Point>::component_type Dim;
    std::size_t dims = point_traits<Point>::dimensions(p1);
    for (std::size_t i = 0; i < dims; ++i) {
      Dim too_close = extent[i] / Dim(10000);
      if (fabs(p1[i] - p2[i]) < too_close) {
        Dim dist_to_move = sqrt(extent[i]) / Dim(200);
        if (p1[i] - origin[i] < origin[i] + extent[i] - p1[i])
          p1[i] += dist_to_move * drand48();
        else
          p1[i] -= dist_to_move * drand48();
      }
    }
  }

  template<typename PositionMap, typename DisplacementMap,
           typename RepulsiveForce, typename Graph>
  struct fr_apply_force
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename property_traits<PositionMap>::value_type Point;
    typedef typename point_traits<Point>::component_type Dim;

    fr_apply_force(const PositionMap& position,
                   const DisplacementMap& displacement,
                   Point origin, Point extent,
                   RepulsiveForce repulsive_force, Dim k, const Graph& g)
      : position(position), displacement(displacement), origin(origin),
        extent(extent), repulsive_force(repulsive_force), k(k), g(g)
    { }

    void operator()(vertex_descriptor u, vertex_descriptor v)
    {
#ifndef BOOST_NO_STDC_NAMESPACE
      using std::sqrt;
#endif // BOOST_NO_STDC_NAMESPACE
      if (u != v) {
        // When the vertices land on top of each other, move the
        // first vertex away from the boundaries.
        maybe_jitter_point(position[u], position[v], origin, extent);

        // DPG TBD: Can we use the Topology concept's
        // distance/move_position_toward to handle this?
        Dim delta_x = position[v][0] - position[u][0];
        Dim delta_y = position[v][1] - position[u][1];
        Dim dist = sqrt(delta_x * delta_x + delta_y * delta_y);
        if (dist == Dim(0)) {
          displacement[v][0] += 0.01;
          displacement[v][1] += 0.01;
        } else {
          Dim fr = repulsive_force(u, v, k, dist, g);
          displacement[v][0] += delta_x / dist * fr;
          displacement[v][1] += delta_y / dist * fr;
        }
      }
    }

  private:
    PositionMap position;
    DisplacementMap displacement;
    Point origin;
    Point extent;
    RepulsiveForce repulsive_force;
    Dim k;
    const Graph& g;
  };

} // end namespace detail

template<typename Graph, typename PositionMap, 
         typename AttractiveForce, typename RepulsiveForce,
         typename ForcePairs, typename Cooling, typename DisplacementMap>
void
fruchterman_reingold_force_directed_layout
 (const Graph&    g,
  PositionMap     position,
  typename property_traits<PositionMap>::value_type const& origin,
  typename property_traits<PositionMap>::value_type const& extent,
  AttractiveForce attractive_force,
  RepulsiveForce  repulsive_force,
  ForcePairs      force_pairs,
  Cooling         cool,
  DisplacementMap displacement)
{
  typedef typename property_traits<PositionMap>::value_type Point;
  typedef typename point_traits<Point>::component_type Dim;
  typedef typename graph_traits<Graph>::vertex_iterator   vertex_iterator;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_iterator     edge_iterator;

#ifndef BOOST_NO_STDC_NAMESPACE
  using std::sqrt;
#endif // BOOST_NO_STDC_NAMESPACE

  std::size_t num_dimensions = point_traits<Point>::dimensions(origin);
  Dim volume = std::accumulate(&extent[0], &extent[0] + num_dimensions,
                               Dim(1), std::multiplies<Dim>());

  // assume positions are initialized randomly
  Dim k = sqrt(volume / num_vertices(g));

  detail::fr_apply_force<PositionMap, DisplacementMap,
                         RepulsiveForce, Graph>
    apply_force(position, displacement, origin, extent, repulsive_force, k, g);

  do {
    // Calculate repulsive forces
    vertex_iterator v, v_end;
    for (tie(v, v_end) = vertices(g); v != v_end; ++v)
      displacement[*v] = Point();
    force_pairs(g, apply_force);

    // Calculate attractive forces
    edge_iterator e, e_end;
    for (tie(e, e_end) = edges(g); e != e_end; ++e) {
      vertex_descriptor v = source(*e, g);
      vertex_descriptor u = target(*e, g);

      // When the vertices land on top of each other, move the
      // first vertex away from the boundaries.
      ::boost::detail::maybe_jitter_point(position[u], position[v], 
                                          origin, extent);

      // DPG TBD: Can we use the Topology concept's
      // distance/move_position_toward to handle this?
      Dim delta_x = position[v][0] - position[u][0];
      Dim delta_y = position[v][1] - position[u][1];
      Dim dist = sqrt(delta_x * delta_x + delta_y * delta_y);
      Dim fa = attractive_force(*e, k, dist, g);

      displacement[v][0] -= delta_x / dist * fa;
      displacement[v][1] -= delta_y / dist * fa;
      displacement[u][0] += delta_x / dist * fa;
      displacement[u][1] += delta_y / dist * fa;
    }

    if (Dim temp = cool()) {
      // Update positions
      for (tie(v, v_end) = vertices(g); v != v_end; ++v) {
        BOOST_USING_STD_MIN();
        BOOST_USING_STD_MAX();
        Dim disp_size = sqrt(displacement[*v][0] * displacement[*v][0]
                             + displacement[*v][1] * displacement[*v][1]);
        for (std::size_t dim = 0; dim < num_dimensions; ++dim) {
          position[*v][dim] += displacement[*v][dim] / disp_size 
                             * (min)(disp_size, temp);
          if (vertex_migration) {
            position[*v][dim] = (min)(1.0f, 
                                      (max)(-1.0f, position[*v][dim]));
          } else {
            position[*v][dim] = (min)(origin[dim] + extent[dim], 
                                      (max)(origin[dim], position[*v][dim]));
          }
          
          // CEM HACK: Jitter if we're on the edges
          if(position[*v][dim] == 1.0f) // origin[dim] + extent[dim])
            position[*v][dim] -= drand48() * .1 * extent[dim];
          else if(position[*v][dim] == -1.0f) // origin[dim])
            position[*v][dim] += drand48() * .1 * extent[dim];
        }
      }
    } else {
      break;
    }
  } while (true);
}

namespace detail {
  template<typename DisplacementMap>
  struct fr_force_directed_layout
  {
    template<typename Graph, typename PositionMap, 
             typename AttractiveForce, typename RepulsiveForce,
             typename ForcePairs, typename Cooling,
             typename Param, typename Tag, typename Rest>
    static void
    run(const Graph&    g,
        PositionMap     position,
        typename property_traits<PositionMap>::value_type const& origin,
        typename property_traits<PositionMap>::value_type const& extent,
        AttractiveForce attractive_force,
        RepulsiveForce  repulsive_force,
        ForcePairs      force_pairs,
        Cooling         cool,
        DisplacementMap displacement,
        const bgl_named_params<Param, Tag, Rest>&)
    {
      fruchterman_reingold_force_directed_layout
        (g, position, origin, extent, attractive_force, repulsive_force,
         force_pairs, cool, displacement);
    }
  };

  template<>
  struct fr_force_directed_layout<error_property_not_found>
  {
    template<typename Graph, typename PositionMap, 
             typename AttractiveForce, typename RepulsiveForce,
             typename ForcePairs, typename Cooling,
             typename Param, typename Tag, typename Rest>
    static void
    run(const Graph&    g,
        PositionMap     position,
        typename property_traits<PositionMap>::value_type const& origin,
        typename property_traits<PositionMap>::value_type const& extent,
        AttractiveForce attractive_force,
        RepulsiveForce  repulsive_force,
        ForcePairs      force_pairs,
        Cooling         cool,
        error_property_not_found,
        const bgl_named_params<Param, Tag, Rest>& params)
    {
      typedef typename property_traits<PositionMap>::value_type Point;
      std::vector<Point> displacements(num_vertices(g));
      fruchterman_reingold_force_directed_layout
        (g, position, origin, extent, attractive_force, repulsive_force,
         force_pairs, cool,
         make_iterator_property_map
         (displacements.begin(),
          choose_const_pmap(get_param(params, vertex_index), g,
                            vertex_index),
          Point()));
    }
  };

} // end namespace detail

template<typename Graph, typename PositionMap, typename Param,
         typename Tag, typename Rest>
void
fruchterman_reingold_force_directed_layout
  (const Graph&    g,
   PositionMap     position,
   typename property_traits<PositionMap>::value_type const& origin,
   typename property_traits<PositionMap>::value_type const& extent,
   const bgl_named_params<Param, Tag, Rest>& params)
{
  typedef typename property_value<bgl_named_params<Param,Tag,Rest>,
                                  vertex_displacement_t>::type D;

  detail::fr_force_directed_layout<D>::run
    (g, position, origin, extent,
     choose_param(get_param(params, attractive_force_t()),
                  square_distance_attractive_force()),
     choose_param(get_param(params, repulsive_force_t()),
                  square_distance_repulsive_force()),
     choose_param(get_param(params, force_pairs_t()),
                  make_grid_force_pairs(origin, extent, position, g)),
     choose_param(get_param(params, cooling_t()),
                  linear_cooling<double>(100)),
     get_param(params, vertex_displacement_t()),
     params);
}

template<typename Graph, typename PositionMap>
void
fruchterman_reingold_force_directed_layout
  (const Graph&    g,
   PositionMap     position,
   typename property_traits<PositionMap>::value_type const& origin,
   typename property_traits<PositionMap>::value_type const& extent)
{
  fruchterman_reingold_force_directed_layout
    (g, position, origin, extent,
     attractive_force(square_distance_attractive_force()));
}

} // end namespace boost

#ifdef BOOST_GRAPH_IS_PARALLEL
#  include <boost/graph/distributed/fruchterman_reingold.hpp>
#endif // BOOST_GRAPH_IS_PARALLEL

#endif // BOOST_GRAPH_FRUCHTERMAN_REINGOLD_FORCE_DIRECTED_LAYOUT_HPP
