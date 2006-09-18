import boost.graph as bgl

g = bgl.Graph((('a', 'b'), ('b', 'c'), ('c', 'd'), ('d', 'a')), 'name')

print 'When not hidden, contains',len(bgl.biconnected_components(g, g.add_edge_property(type='integer'))),'articulation points'

hide_edges = g.add_edge_property(type='integer')
hide_edges[g.edges.next()] = 1

print 'When hidden, contains',len(bgl.biconnected_components(g, g.add_edge_property(type='integer'), hide_edges)),'articulation points'

g.remove_edge(g.edges.next())
print 'When removed, contains',len(bgl.biconnected_components(g, g.add_edge_property(type='integer'))),'articulation points'
