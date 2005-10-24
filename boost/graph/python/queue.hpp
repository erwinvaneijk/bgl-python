// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_PYTHON_QUEUE_HPP
#define BOOST_GRAPH_PYTHON_QUEUE_HPP

#include <boost/python.hpp>

namespace boost { namespace graph { namespace python {

using boost::python::object;

template<typename Queue>
class maybe_python_queue
{
public:
  typedef typename Queue::value_type value_type;

  maybe_python_queue(object python_Q, const Queue& Q) 
    : python_Q(python_Q), Q(Q) { }

  bool empty() const    
  { 
    using boost::python::extract;
    if (python_Q == object()) return Q.empty();
    else return extract<bool>(python_Q.attr("empty")());
  }

  value_type top() const         
  { 
    using boost::python::extract;
    if (python_Q == object()) return Q.top();
    else return extract<value_type>(python_Q.attr("top")());
  }

  void pop()
  { 
    if (python_Q == object()) Q.pop();
    else python_Q.attr("pop")();
  }

  void push(const value_type& x)
  {
    if (python_Q == object()) Q.push(x);
    else python_Q.attr("push")(x);
  }    

  void update(const value_type& x)
  {
    if (python_Q == object()) Q.update(x);
    else python_Q.attr("update")(x);
  }

private:
  object python_Q;
  Queue Q;
};

} } } // end namespace boost::graph::python

#endif // BOOST_GRAPH_PYTHON_QUEUE_HPP
