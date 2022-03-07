import gtt
from gtt import *
from gtt.render.ascii import StateCache

#from json import json

class Settings:
    def __init__(self, track: gtt.Track):
        self.measure_buffer = 0
        self.active_measure_width = 8 * track.time_signature.beats_per_measure()
        #self.total_measure_width = self.active_measure_width+(2*self.measure_buffer)

        self.m_per_row = 3
        self.row_gap = 3

        self.x_gap = 6

        #self.beat_size = self.active_measure_width / track.time_signature.beats_per_measure()

        self.possible_modes = [ "VIEW", "EDIT", "ADD_NOTES" ]
        self.mode_index = 0
        #self.mode = self.possible_modes[0]x

        # UNSERIALIZED:
        #self.state_cache = StateCache()

    def mode_str( self ):
        return self.possible_modes[ self.mode_index ]

    def toggle_mode( self ):
        self.mode_index = (self.mode_index+1) % len(self.possible_modes)

    def serialize( self ):
        return {
            "self.measure_buffer" : self.measure_buffer,
            "self.active_measure_width" : self.active_measure_width,
            "self.m_per_row" : self.m_per_row,
            "self.row_gap" : self.row_gap,
            "self.x_gap" : self.x_gap,
            "self.mode" : self.possible_modes[ self.mode_index ]
        }

    
    def deserialize( self, settings_json ):
        if "self.measure_buffer" in settings_json:
            self.measure_buffer = settings_json[ "self.measure_buffer" ]
        if "self.active_measure_width" in settings_json:
            self.active_measure_width = settings_json[ "self.active_measure_width" ]
        if "self.m_per_row" in settings_json:
            self.m_per_row = settings_json[ "self.m_per_row" ]
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
