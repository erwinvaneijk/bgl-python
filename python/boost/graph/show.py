import wx
import wx.glcanvas
import sys
import math
from sets import Set
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import Numeric

import random
import boost.graph

DEFAULT_RADIUS = 1.0
DEFAULT_SLICES = 20

LINE_WIDTH = 1.0

def show(g, position_map = None):
    # If we didn't receive a position map, layout the graph...
    if position_map == None:
        position_map = g.add_vertex_property(type='point2d')
        if (not g.is_directed() and boost.graph.connected_components(g) == 1 and
            g.num_vertices() < 100):
            boost.graph.kamada_kawai_spring_layout(g, position_map)
        else:
            boost.graph.fruchterman_reingold_force_directed_layout(g, position_map)
        
    app = wx.PySimpleApp()
    frame = wx.Frame(None, title="")
    canvas = MiniOpenGLGraphCanvas(frame, g, position_map)
    frame.Show()
    app.MainLoop()







class glPosition(object):
    def __init__(self, pos=(0.0, 0.0)):
        self.x = pos[0]
        self.y = pos[1]
        if len(pos)==3:
            self.z = pos[2]
        else:
            self.z = 0.0

class Callable:
    def __init__(self, anycallable):
        self.__call__ = anycallable
           
class MiniOpenGLGraphCanvas(wx.Panel):    
    def __init__(self, parent, 
                 graph, position_map):
#                 model = None):
        size=(400, 400)
        title=""
        id = wx.ID_ANY

        self.width = size[0]
        self.height = size[1]
#        self.model = model
        self.graph = graph
        self.position_map = position_map


        wx.Panel.__init__(self, parent = parent, id = id, style = wx.NO_BORDER)

        self.glcanvas = wx.glcanvas.GLCanvas(self, -1, 
                                      size=(self.width, self.height))
        
        self.fogOn = False
        self.orthographic = True
        self.drawAxes = True

        self.vertex_data = []
        self.vertex_color_data = None
        self.edge_data = []
        
        self._initialize_drawing_parameters()

        if "windows" in sys.platform:
            # Windows seems to be upside down...
            self.top_high = False
        else:
            self.top_high = True

        glEnable(GL_COLOR_MATERIAL)
        glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION)

        glEnable(GL_POINT_SMOOTH)

        glPointSize(5.0)
        glLineWidth(1.0)

        glEnable(GL_DEPTH_TEST)

        if self.fogOn:
            glEnable(GL_FOG)
            glFog(GL_FOG_COLOR, [0.5, 0.5, 0.5, 1.0])
            glFog(GL_FOG_MODE, GL_EXP)
            glHint(GL_FOG_HINT, GL_DONT_CARE)
            glFog(GL_FOG_DENSITY, 0.005)

        glClearColor(0.5, 0.5, 0.5, 1.0)
        glClearDepth(1.0)

        self.Resize(self.width, self.height)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        self.setViewport()


        self.glcanvas.Bind(wx.EVT_PAINT, self.OnPaint)
        self.glcanvas.Bind(wx.EVT_MOTION, self.OnMotion)
        self.glcanvas.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
        self.glcanvas.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
        self.glcanvas.Bind(wx.EVT_CHAR, self.OnChar)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_CLOSE, self.OnClose)

        if self.graph is not None:
            if self.position_map is None:
                self.position_map = self.graph.add_vertex_property(type='point2d')
                boost.graph.circle_graph_layout(self.graph, self.position_map, 50)
                model.SetVertexProperty('position', self.position_map)
            self.set_graph(self.graph, self.position_map)
                
    def _initialize_drawing_parameters(self):
        self.angle_x = 0
        self.angle_y = 0
        self.delta_x = 0
        self.delta_y = 0
        self.bottom = -1.0
        self.left = -1.0
        self.top = 1.0
        self.left, self.top, self.right, self.bottom = self.compute_rect()
        self.active_vertex = None
        self.zoomed = False
        self.selection = None
        self.dragging = False        

    def set_graph(self, graph, position_map, property_maps={}):
        self.graph = graph
        self.position_map = position_map
        self.ref_3d_map = self.graph.add_vertex_property(type="point3d") # using this to check if position map is point3d
        self.property_maps = property_maps
        self.vertex_position_rect = self.compute_rect()

        self.update_layout()

        self.active_vertex = None

        # Set initial view to cover whole graph
        self.vertex_position_rect = self.compute_rect()
        (left, top, right, bottom) = self.compute_rect()
        self.left = left
        self.right = right
        self.bottom = bottom
        self.top = top
        
        self.Draw()

    def compute_rect(self):
        pos = self.position_map
        if self.graph == None or self.graph.num_vertices() == 0:
          return (-1, -1, 50, 50)
        else:
          left, top, right, bottom = 0, 0, 0, 0
          for v in self.graph.vertices:
              if pos[v][0] < left: left = pos[v][0]
              if pos[v][1] > top: top = pos[v][1]
              if pos[v][0] > right: right = pos[v][0]
              if pos[v][1] < bottom: bottom = pos[v][1]
          return (left, top, right, bottom)

    def update_layout(self):
        self.vertex_position_rect = self.compute_rect()
        (left, top, right, bottom) = self.compute_rect()
        self.left = left
        self.right = right
        self.bottom = bottom
        self.top = top
        self.scale = (right-left)/self.width
        self.vertex_data = Numeric.zeros((self.graph.num_vertices(), 3), Numeric.Float)
        self.edge_data = Numeric.zeros((self.graph.num_edges() * 2, 3), Numeric.Float)
        index = 0
        for v in self.graph.vertices:
            self.vertex_data[index, 0] = self.position_map[v][0]
            self.vertex_data[index, 1] = self.position_map[v][1]
            if type(self.position_map) == type(self.ref_3d_map):
                self.vertex_data[index, 2] = self.position_map[v][2]
            else:
                self.vertex_data[index, 2] = 0
            index = index + 1

        index = 0
        for e in self.graph.edges:
            src_pos = self.position_map[self.graph.source(e)]
            tgt_pos = self.position_map[self.graph.target(e)]
            self.edge_data[index, 0] = src_pos[0]
            self.edge_data[index, 1] = src_pos[1]
            self.edge_data[index + 1, 0] = tgt_pos[0]
            self.edge_data[index + 1, 1] = tgt_pos[1]
            if type(self.position_map) == type(self.ref_3d_map):
                self.edge_data[index, 2] = src_pos[2]
                self.edge_data[index + 1, 2] = tgt_pos[2]
            else:
                self.edge_data[index, 2] = 0
                self.edge_data[index + 1, 2] = 0
            index = index + 2

        self.Draw()

    def _generate_vertex_colors(self, n, type=GL_RGBA):
        colors = []
        for i in range(1, n+1):
            colors.append((0.0, 0.0, 0.0 + 1.0*(i/float(n)), 1.0))
        return colors

    def OnClose(self, event):
        if event.CanVeto():
            pass
#        self.Destroy()
        event.Skip()

    def setViewport(self):
        if self.IsShown():
            self.glcanvas.SetCurrent()
        # self.Show(True)
        # self.glcanvas.SetCurrent()
        if not self.top_high:
            # Windows seems to be upside down...
            glViewport(0, self.height, self.width, self.height)
        else:
            glViewport(0, 0, self.width, self.height)
            # glViewport(x, y, height, width) - x,y, is BOTTOM LEFT corner

    def Resize(self, width, height):
        self.width = width
        self.height = height

        if self.IsShown():
            self.glcanvas.SetCurrent()
            self.glcanvas.SetSize((self.width, self.height))
        
        # self.vertex_position_rect = self.compute_rect()
        # (left, top, right, bottom) = self.compute_rect()
        # self.left = left
        # self.bottom = bottom
        # self.top = top
        left = self.left
        right = self.right
        top = self.top
        bottom = self.bottom
        
        if width == 0 or height == 0:
            return

        if math.fabs(right-left)/float(width) > math.fabs(top-bottom)/self.height:
            width_dominant = True
        else:
            width_dominant = False
        if width_dominant:
            self.scale = math.fabs(right-left)/float(width)*1.2
            self.pad = 0.1*math.fabs(right-left)
        else:
            self.scale = math.fabs(top-bottom)/float(height)*1.2
            self.pad = 0.1*math.fabs(top-bottom)

        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        if self.zoomed == False:
            if self.orthographic:
                glOrtho(self.left - self.pad, self.left - self.pad + self.scale*self.width, \
                        self.bottom + - self.pad + self.scale*self.height, self.bottom - self.pad, \
                        max(self.width*self.scale, self.height*self.scale), \
                        2*max(self.width*self.scale, self.height*self.scale))
            else:
                glFrustum(2.0*(self.left - self.pad)/3.0, 2.0*(self.left - + self.pad + self.scale*self.width)/3.0, \
                          2.0*(self.bottom  - self.pad + self.scale*self.height)/3.0, 2.0*(self.bottom - self.pad)/3.0, \
                          max(self.width*self.scale, self.height*self.scale),\
                          2*max(self.width*self.scale, self.height*self.scale))
        else:
            glOrtho(self.zoom_left, self.zoom_right, self.zoom_top, self.zoom_bottom, self.zoom_front, self.zoom_back) 

        if self.fogOn:
            glFog(GL_FOG_START, 1.5*max(self.width*self.scale, self.height*self.scale))
            glFog(GL_FOG_END, 2*max(self.width*self.scale, self.height*self.scale))

        self.setViewport()
        glMatrixMode(GL_MODELVIEW)

    def Draw(self):
        self.Resize(self.width, self.height)

        if self.IsShown():
            self.glcanvas.SetCurrent()

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glTranslatef(self.delta_x, self.delta_y, -1.5*max(self.width*self.scale, self.height*self.scale))
        glRotatef(math.degrees(self.angle_x), 0, 1, 0)
        glRotatef(math.degrees(self.angle_y), 1, 0, 0)

        if self.drawAxes:
            glBegin(GL_LINES)
            glVertex3f(0.0, 0.0, 0.0)
            glVertex3f(10.0, 0.0, 0.0)
            glVertex3f(0.0, 0.0, 0.0)
            glVertex3f(0.0, 10.0, 0.0)
            glVertex3f(0.0, 0.0, 0.0)
            glVertex3f(0.0, 0.0, 10.0)
            glEnd()

        if self.graph != None and len(self.vertex_data) > 0:
            glEnableClientState(GL_VERTEX_ARRAY)

            # This shows each point
            glPointSize(5.0)
            if self.vertex_color_data != None and len(self.vertex_color_data) == len(self.vertex_data):
                color_array = True
            else:
                color_array = False
                glColor4f(0.0, 0.0, 1.0, 1.0)
            if color_array:
                glColorPointerf(self.vertex_color_data)
                glEnableClientState(GL_COLOR_ARRAY)
            glVertexPointerf(self.vertex_data)
            glDrawArrays(GL_POINTS, 0, len(self.vertex_data))            

            # This puts a green ring around each point.
            #glPointSize(8.0)
            #glColor4f(0.0, 1.0, 0.0, 1.0)
            #glVertexPointerf(self.vertex_data)
            #glDrawArrays(GL_POINTS, 0, len(self.vertex_data))

            # This draws the edges
            if color_array:
                glDisableClientState(GL_COLOR_ARRAY)
            glColor4f(1.0, 0.0, 0.0, 1.0)
            glVertexPointerf(self.edge_data)
            glDrawArrays(GL_LINES, 0, len(self.edge_data))
            glDisableClientState(GL_VERTEX_ARRAY)

            if self.graph.is_directed():
                (left, top, right, bottom) = self.compute_rect()
                delta = right-left
                fraction = 0.03
                for e in self.graph.edges:
                    u = self.graph.source(e)
                    v = self.graph.target(e)
                    u_loc = Numeric.asarray(self.position_map[u])
                    v_loc = Numeric.asarray(self.position_map[v])
                    direction = Numeric.zeros((3,))
                    direction[0] = v_loc[0] - u_loc[0]
                    direction[1] = v_loc[1] - u_loc[1]
                    if len(v_loc) > 2:
                        direction[2] = v_loc[2] - u_loc[2]
                    direction_norm = math.sqrt(sum(direction*direction))         
                    try:
                        direction = direction / direction_norm
                    except:
                        pass
                    if len(v_loc) > 2:
                        p = Numeric.asarray((0, 0, 0))
                    else:
                        p = Numeric.asarray((0, 0))                    
                    p = Numeric.asarray((0, 0, 0)) #
                    p[0] = v_loc[0] - delta*fraction*direction[0]
                    p[1] = v_loc[1] - delta*fraction*direction[1]
                    p[2] = 0 - delta*fraction*direction[2] #
                    if len(v_loc) > 2:
                        p[2] = v_loc[2] - delta*fraction*direction[2]
                    p[2] = 0 - delta*fraction*direction[2] #

                    o = Numeric.asarray((-1.0*direction[1], direction[0], 0)) # p x <0,0,-1>
                    o = o / math.sqrt(sum(o*o))
                        
                    glBegin(GL_TRIANGLES)
                    glVertex3f(p[0] + delta*fraction*0.25*o[0], \
                               p[1] + delta*fraction*0.25*o[1], \
                               p[2] + delta*fraction*0.25*o[2])
                    if len(v_loc) > 2:
                        glVertex3f(v_loc[0], v_loc[1], v_loc[2])
                    else:
                        glVertex3f(v_loc[0], v_loc[1], 0)
                    glVertex3f(p[0] - delta*fraction*0.25*o[0], \
                               p[1] - delta*fraction*0.25*o[1], \
                               p[2] - delta*fraction*0.25*o[2])
                    glEnd()

                    o2 = Numeric.asarray((p[1]*o[2] - p[2]*o[1],\
                                          -1.0*(p[0]*o[2] - p[2]*o[0]),\
                                          p[0]*o[1] - p[1]*o[0])) # p x o
#                    print o2, "(", o, ")"
                    try:
                        o2 = o2 / math.sqrt(sum(o2*o2))
                    except:
                        pass
                    glBegin(GL_TRIANGLES)
                    glVertex3f(p[0] + delta*fraction*0.25*o2[0], \
                               p[1] + delta*fraction*0.25*o2[1], \
                               p[2] + delta*fraction*0.25*o2[2])
                    if len(v_loc) > 2:
                        glVertex3f(v_loc[0], v_loc[1], v_loc[2])
                    else:
                        glVertex3f(v_loc[0], v_loc[1], 0)
                    glVertex3f(p[0] - delta*fraction*0.5*o[0], \
                               p[1] - delta*fraction*0.5*o[1], \
                               p[2] - delta*fraction*0.5*o[2])                    
                    glEnd()

                   
        if self.active_vertex != None:
            pos = self.position_map[self.active_vertex]
            glDisable(GL_LIGHTING)
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            z = 0.
            if len(pos) == 3:
                z = pos[2]
            glTranslatef(pos[0], pos[1], z)
            glColorf(1.0, 1.0, 0.0, 0.5)
            glutSolidSphere(DEFAULT_RADIUS*1.5, DEFAULT_SLICES, DEFAULT_SLICES)
            glDisable(GL_BLEND)
            glEnable(GL_LIGHTING)

        if self.selection != None:
            selection = self.selection
            sel_left = min(selection[0].x, selection[1].x)
            sel_right = max(selection[0].x, selection[1].x)
            sel_bottom = min(selection[0].y, selection[1].y)
            sel_top = max(selection[0].y, selection[1].y)
            glColor4f(0.5, 0.5, 0.5, .25)
            glEnable(GL_COLOR_LOGIC_OP)
            glLogicOp(GL_XOR)
            glBegin(GL_LINE_LOOP)
            glVertex(sel_left, sel_bottom)
            glVertex(sel_left, sel_top)
            glVertex(sel_right, sel_top)
            glVertex(sel_right, sel_bottom)
            glEnd()    
            glDisable(GL_COLOR_LOGIC_OP)
            self.selection = None

        self.glcanvas.SwapBuffers()

    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.Draw()
        event.Skip()

    def OnMotion(self, event):
        raw_pos = event.GetPosition()
        pos = self.translatePos(raw_pos)
        if event.Dragging():
            self.dragging = True
            j = self.active_vertex
            if j != None:
                self.position_map[j] = boost.graph.Point2D(pos.x, pos.y)
                self.update_layout()
        if event.Dragging() and event.ShiftDown():
            delta = (pos.x - self.drag_pos_start.x, pos.y - self.drag_pos_start.y)
            self.delta_x = delta[0]
            self.delta_y = delta[1]
            self.Draw()
        # Have to use CmdDown on OS X since EVT_LEFT_DOWN is not set when CTRL is down
        if event.Dragging() and (event.ControlDown() or event.CmdDown()):
            self.drag_pos_end = pos
            self.selection = (self.drag_pos_start, self.drag_pos_end)
            self.Draw()
        event.Skip()

    def OnLeftDown(self, event):
        raw_pos = event.GetPosition()
        pos = self.translatePos(raw_pos)
        j = self.detectSelection(pos)
        if j != None:
            self.active_node = j
            self.active_vertex = j
        else:
            self.active_node = None
            self.active_vertex = None          
        if not self.active_vertex:
            self.drag_pos_start = pos
        self.Draw()
        event.Skip()

    def OnLeftUp(self, event):
        if self.active_vertex != None:
            self.active_vertex = None
            self.Draw()
        if self.dragging and (event.ControlDown() or event.CmdDown()):
            raw_pos = event.GetPosition()
            pos = self.translatePos(raw_pos)
            self.drag_pos_end = pos
            self.left = min(self.drag_pos_start.x, self.drag_pos_end.x)
            self.right = max(self.drag_pos_start.x, self.drag_pos_end.x)
            self.bottom = min(self.drag_pos_start.y, self.drag_pos_end.y)
            self.top = max(self.drag_pos_start.y, self.drag_pos_end.y)
            self.selection = None
            self.Draw()
        self.dragging = False
        event.Skip()

    def translatePos(self, raw_pos):
        pos = glPosition()
        pos.x = self.scale * raw_pos.x + self.left - self.pad
        pos.y = self.scale * raw_pos.y + self.bottom - self.pad
        return pos

    def detectSelection(self, pos):
        for node in self.graph.vertices:
            node_pos = self.position_map[node]
            if (pow(pos.x-node_pos[0], 2) + pow(pos.y-node_pos[1], 2)) <= pow(DEFAULT_RADIUS, 2):
                return node
        return None

    def OnChar(self, event):
        needsRedraw = True
        key = event.GetKeyCode()
        if not event.ShiftDown():
            if key == wx.WXK_LEFT:
                self.angle_x = self.angle_x - math.pi*0.125
            elif key ==  wx.WXK_RIGHT:
                self.angle_x = self.angle_x + math.pi*0.125
            elif key == wx.WXK_UP:
                self.angle_y = self.angle_y + math.pi*0.125
            elif key == wx.WXK_DOWN:
                self.angle_y = self.angle_y - math.pi*0.125
            elif key == ord('r'):
                self._initialize_drawing_parameters()
        elif event.ShiftDown():
            if key == wx.WXK_LEFT:
                self.delta_x -= 0.5
            elif key ==  wx.WXK_RIGHT:
                self.delta_x += 0.5
            elif key == wx.WXK_UP:
                self.delta_y += 0.5
            elif key == wx.WXK_DOWN:
                self.delta_y -= 0.5
        else:
            needsRedraw = False

        if needsRedraw:
            self.Draw()
        
    def OnSize(self, event):
        # event gives us total size, but we can't use all of it
        self.size = self.GetClientSize()
        self.Resize(self.size[0], self.size[1])
        self.Draw()
        event.Skip()

#    def MenuName():
#      return "Node/Edge (OpenGL)"
#    MenuName = Callable(MenuName)
#    
#    def name():
#        return 'Node/Edge View'
#    name = Callable(name)
#
#    def accepts(data):
#        return type(data) == boost.graph.Graph or type(data) == boost.graph.Digraph
#    accepts = Callable(accepts)

    def GetImage(self):
        pixels = glReadPixelsub(0, 0, self.size[0], self.size[1], GL_RGBA)
        return (self.size, pixels)
