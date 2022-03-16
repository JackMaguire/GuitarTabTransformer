import gtt
from gtt import *
from gtt.render.ascii import StateCache

#from json import json

class Settings:
    def __init__(self, track: gtt.Track, stdscr ):
        self.stdscr = stdscr

        self.active_measure_width = 8 * track.time_signature.beats_per_measure()

        #self.m_per_row = 3
        self.row_gap = 2
        self.x_gap = 6
        self.n_rows_skipped = 0

        self.possible_modes = [ "VIEW", "EDIT", "ADD_NOTES", "EDIT_MEASURES" ]
        self.mode_index = 0

    def move_cursor( self, dy, dx=0 ):
        y, x = self.stdscr.getyx()
        y = y+dy
        x = x+dx

        maxy, maxx = self.stdscr.getmaxyx()
        if y > maxy-1: y = maxy-1
        elif y < 1: y = 1
        if x > maxx-1: x = maxx-1
        elif x < 1: x = 1

        self.stdscr.move( y, x )

    def mode_str( self ):
        return self.possible_modes[ self.mode_index ]

    def toggle_mode( self ):
        self.mode_index = (self.mode_index+1) % len(self.possible_modes)

    def serialize( self ):
        return {
            "self.active_measure_width" : self.active_measure_width,
            "self.n_rows_skipped" : self.n_rows_skipped,
            "self.row_gap" : self.row_gap,
            "self.x_gap" : self.x_gap,
            "self.mode" : self.possible_modes[ self.mode_index ]
        }

    
    def deserialize( self, settings_json ):
        if "self.active_measure_width" in settings_json:
            self.active_measure_width = settings_json[ "self.active_measure_width" ]
        if "self.n_rows_skipped" in settings_json:
            self.n_rows_skipped = settings_json[ "self.n_rows_skipped" ]
        if "self.row_gap" in settings_json:
            self.row_gap = settings_json[ "self.row_gap" ]
        if "self.x_gap" in settings_json:
            self.x_gap = settings_json[ "self.x_gap" ]

        # allow for list to change over time
        if "self.mode" in settings_json:
            mode = settings_json[ "self.mode" ]
        elif "self.possible_modes" in settings_json and "self.mode_index" in settings_json:
            old_modes = settings_json[ "self.possible_modes" ]
            mode = old_modes[ settings_json[ "self.mode_index" ] ]
        else:
            mode = self.possible_modes[0]

        for i in range( 0, len(self.possible_modes) ):
            if self.possible_modes[i] == mode:
                self.mode_index = i
