# Copyright (C) 2006 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine
from boost.graph import *

g = Graph()
v = g.add_vertex()

strings = g.add_vertex_property(type='string')
colors = g.add_vertex_property(type='color')


cannot_parse_property_okay = False
try:
    strings[v] = 'boo'
    strings.astype('float')
except CannotParseProperty, e:
    print e
    cannot_parse_property_okay = True

assert cannot_parse_property_okay

invalid_property_conversion_okay = False
try:
    colors.astype('float')
except InvalidPropertyConversion, e:
    print e
    invalid_property_conversion_okay = True

assert invalid_property_conversion_okay
