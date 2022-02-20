import gtt
from gtt import *

class Settings:
    def __init__(self, track: gtt.Track):
        self.measure_buffer = 0
        self.active_measure_width = 8 * track.time_signature.beats_per_measure()
        self.total_measure_width = self.active_measure_width+(2*self.measure_buffer)

        self.m_per_row = 3
        self.row_gap = 3

        self.x_gap = 6

        self.beat_size = self.active_measure_width / track.time_signature.beats_per_measure()

    def x_is_beat( self, x ):
        if x < self.measure_buffer: return False

        x = x - self.measure_buffer

        if x >= self.active_measure_width: return False

        return x % self.beat_size == 0
