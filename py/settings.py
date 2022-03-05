import gtt
from gtt import *

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
            "self.possible_modes" : self.possible_modes,
            "self.mode_index" : self.mode_index
        }

    
