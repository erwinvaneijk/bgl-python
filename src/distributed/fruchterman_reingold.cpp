// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#include "config.hpp"
#include "graph_types.hpp"
#include <boost/parallel/property_map.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/python/point2d.hpp>
#include <boost/graph/python/iterator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <ctime>
#include <boost/python.hpp>

namespace boost { namespace graph { namespace distributed { namespace python {

using boost::graph::python::point2d;

template<typename F, typename Result = typename F::result_type>
struct python_or_functor
{
  typedef Result result_type;

  explicit python_or_functor(boost::python::object callable, const F& f = F()) 
    : callable(callable), f(f), registered_apply_force(false) { }

  // For cooling
  result_type operator()()
  {
    using boost::python::object;
    using boost::python::extract;
    if (callable != object()) return extract<result_type>(callable());
    else return f();
  }

  // For force pairs
  template<typename Graph, typename ApplyForce>
  void operator()(const Graph& g, ApplyForce apply_force)
  {
    using boost::python::extract;
    using boost::python::object;
    using boost::graph::python::detail::type_already_registered;

    if (callable != object()) {
      if (!registered_apply_force && !type_already_registered<ApplyForce>())
        {
          using boost::python::class_;
          using boost::python::no_init;
          typedef typename graph_traits<Graph>::vertex_descriptor
            vertex_descriptor;

          typedef void (ApplyForce::* apply_force_call_t)(vertex_descriptor, 
                                                          vertex_descriptor);

          class_<ApplyForce>("ApplyForce", no_init)
            .def("__call__", (apply_force_call_t)&ApplyForce::operator());
                 
          registered_apply_force = true;
        }

      if (extract<bool>(callable(ref(g), apply_force)))
        f(g, apply_force);
    }
    else f(g, apply_force);
  }

  // For the attractive_force
  template<typename Graph>
  result_type 
  operator()(typename graph_traits<Graph>::edge_descriptor e,
             float k, float dist, const Graph& g) const
  {
    using boost::python::object;
    using boost::python::extract;
    if (callable != object()) 
      return extract<result_type>(callable(e, k, dist, ref(g)));
    else 
      return f(e, k, dist, g);
  }

  // For the repulsive_force
  template<typename Graph>
  result_type 
  operator()(typename graph_traits<Graph>::vertex_descriptor u,
             typename graph_traits<Graph>::vertex_descriptor v,
             float k, float dist, const Graph& g) const
  {
    using boost::python::object;
    using boost::python::extract;
    if (callable != object()) 
      return extract<result_type>(callable(u, v, k, dist, ref(g)));
    else return f(u, v, k, dist, g);
  }

private:
  boost::python::object callable;
  F f;
  bool registered_apply_force;
};

template<typename Graph>
void 
fruchterman_reingold_force_directed_layout
  (Graph& g,
   vector_property_map
     <point2d, typename property_map<Graph, vertex_index_t>::const_type>& pos,
   const point2d& origin,
   const point2d& extent,
   boost::python::object attractive_force,
   boost::python::object repulsive_force,
   boost::python::object force_pairs,
   boost::python::object cooling,
   bool progressive,
   const simple_tiling& tiling)
{
  using boost::python::object;

  typedef typename property_map<Graph, vertex_index_t>::const_type
    VertexIndexMap;
  typedef vector_property_map<point2d, VertexIndexMap> PositionMap;

  // Build the displacement map.
  typedef vector_property_map<point2d, VertexIndexMap> DisplacementMap;
  DisplacementMap displacement(num_vertices(g), get(vertex_index, g));

  if (!progressive) {
    minstd_rand gen(std::time(0));
    random_graph_layout(g, pos, origin, extent, gen);
  }

  if (attractive_force != object() || repulsive_force != object()
      || force_pairs != object() || cooling != object()) {
    python_or_functor<square_distance_attractive_force, float>
      fa(attractive_force);
    python_or_functor<square_distance_repulsive_force, float>
      fr(repulsive_force);
    python_or_functor<grid_force_pairs<PositionMap>, void>
      fp(force_pairs, make_grid_force_pairs(origin, extent, pos, g));
    python_or_functor<linear_cooling<float> > local_cool(cooling, 100);

    boost::graph::distributed::fruchterman_reingold_force_directed_layout
      (g, pos, origin, extent, fa, fr, fp, local_cool, displacement, tiling);

  } else {
    boost::graph::distributed::fruchterman_reingold_force_directed_layout
      (g, pos, origin, extent, 
       square_distance_attractive_force(),
       square_distance_repulsive_force(),
       make_grid_force_pairs(origin, extent, pos, g),
       linear_cooling<float>(100), displacement, tiling);
  }
}


void export_fruchterman_reingold_force_directed_layout()
{
  using boost::python::arg;
  using boost::python::class_;
  using boost::python::def;
  using boost::python::init;
  using boost::python::no_init;
  using boost::python::object;

  class_<simple_tiling>("Tiling", no_init)
    .def(init<int, int, bool>());

#define UNDIRECTED_GRAPH(Name,Type)                             \
  def("fruchterman_reingold_force_directed_layout",             \
      &fruchterman_reingold_force_directed_layout<Type>,        \
      (arg("graph"),                                            \
       arg("position"),                                         \
       arg("origin") = point2d(),                               \
       arg("extent") = point2d(500, 500),                       \
       arg("attractive_force") = object(),                      \
       arg("repulsive_force") = object(),                       \
       arg("force_pairs") = object(),                           \
       arg("cooling") = object(),                               \
       arg("progressive") = false,                              \
       arg("tiling") = simple_tiling(1, 1)));
#include "graphs.hpp"
}

} } } } // end namespace boost::graph::distributed::python
