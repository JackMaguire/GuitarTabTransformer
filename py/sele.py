import gtt
from gtt import *

from py.settings import Settings

import curses
from curses.textpad import rectangle

class Selection:
    def __init__( self ):
        self.selected_objects = []
        self.y, self.x = stdscr.getyx()

        # for range selections
        self.box_y = None
        self.box_x = None

    def clear_objs( self ):
        self.selected_objects = []

    def clear_box( self ):
        self.box_y = None
        self.box_x = None

    def maybe_select( self, obj, y, x ) -> bool:
        if self.box_y == None or self.box_x == None:
            if x == self.x and y == self.y:
                self.selected_objects.append( obj )
                return True
            else:
                return False
        else:
            minx = min( self.x, self.box_x )
            maxx = max( self.x, self.box_x )
            miny = min( self.y, self.box_y )
            maxy = max( self.y, self.box_y )
            if x >= minx and x <= maxx and y >= miny and y <= maxy:
                self.selected_objects.append( obj )
                return True
            else:
                return False

    def maybe_select_range( self, obj, y, x, l ) -> bool:
        for i in range( x, x+l ):
            if self.maybe_select( obj, y, x ):
                return True
        return False
            
            
