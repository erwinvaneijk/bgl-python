import boost.graph as bgl
g1 = bgl.read_graphml('../example/data/simple.graphml')
print g1.num_vertices()
g1.write_graphml('simple-clone.graphml')
g2 = bgl.read_graphml('../example/data/attributes.graphml')
g2.write_graphml('attributes-clone.graphml')
