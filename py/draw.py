import gtt
from gtt import *

from gtt.render.ascii import *

from py.settings import Settings

import curses
from curses.textpad import rectangle

#import time

def update_sig_top( track, settings, setting_str ):
    try:
        new_top = int( setting_str.strip().rstrip() )
        track.time_signature.top = new_top
    except:
        pass

def update_m_width( track, settings, setting_str ):
    try:
        new_mw = int( setting_str.strip().rstrip() )
        settings.active_measure_width = new_mw
    except:
        pass

def save_json_please( track, settings, setting_str ):
    try:
        new_mw = int( setting_str.strip().rstrip() )
        settings.active_measure_width = new_mw
    except:
        pass


def add_over_range( topline_actions, x, string, action ):
    for i in range( 0, len( string ) ):
        topline_actions[ x+i ] = action

def draw_text_at_top(
        stdscr,
        track: gtt.Track,
        settings: Settings
):
    topline_actions = {}

    text_color = curses.color_pair(242)
    
    y = 1
    x = settings.x_gap

    # Time Signature
    ts_top_str = str(track.time_signature.top)
    stdscr.addstr( y, x, ts_top_str, text_color )
    add_over_range( topline_actions, x, ts_top_str, update_sig_top )
    x += len(ts_top_str)

    stdscr.addch( y, x, '/', text_color )
    x += 1

    ts_bottom_str = str(track.time_signature.bottom)
    stdscr.addstr( y, x, ts_bottom_str, text_color )
    x += len(ts_bottom_str) + 2

    # Key
    key_str = track.key_str()
    stdscr.addstr( y, x, key_str, text_color )
    x += len(key_str) + 4

    # Width
    w_str = "WIDTH: "
    stdscr.addstr( y, x, w_str, text_color )
    x += len(w_str)
    w_str = str( settings.active_measure_width )
    stdscr.addstr( y, x, w_str, text_color )
    add_over_range( topline_actions, x, w_str, update_m_width )
    x += len(w_str) + 4

    # Mode
    mode_str = "MODE: "
    stdscr.addstr( y, x, mode_str, text_color )
    x += len(mode_str)
    mode_str = settings.mode_str()
    stdscr.addstr( y, x, mode_str, curses.color_pair(3+settings.mode_index) )
    x += len(mode_str) + 4

    

    # Actions
    next_str = "SAVE"
    stdscr.addstr( y, x, next_str, curses.color_pair(2) )
    add_over_range( topline_actions, x, next_str, save_json_please )
    x += len( next_str )

    return topline_actions

def measure_ind_to_xy( index, settings: Settings, mbox_settings, g: gtt.Guitar ):
    j_offset = int( index / settings.m_per_row )
    y = 3 + j_offset*(settings.row_gap+g.size())

    i_offset = index % settings.m_per_row
    x = settings.x_gap + i_offset*(mbox_settings.measure_width+1)

    newline = i_offset==0

    return x, y, newline

class FloatingMeasure:
    def __init__( self, mbox, mbox_settings, start_x, start_y, is_newline ):
        self.mbox = mbox
        self.mbox_settings = mbox_settings
        self.start_x = start_x
        self.start_y = start_y

        self.stop_x_incl = self.start_x + mbox.width() - 1
        self.stop_y_incl = self.start_y + mbox.height() - 1

        self.is_newline = is_newline

    def cursor_is_in_box( self, cursoryx ):
        y = cursoryx[0]
        x = cursoryx[1]
        return y >= self.start_y and y <= self.stop_y_incl and x >= self.start_x and x <= self.stop_x_incl
        

def draw_measure(
        stdscr,
        t: gtt.Track,
        m_index: int,
        settings: Settings,
        cursoryx
):
    noteless_color = curses.color_pair(242)

    m: gtt.Measure = t.measures[ m_index ]
    g: gtt.Guitar =  t.guitar

    mbox_settings = MeasureBoxSettings()
    mbox_settings.time_sig = t.time_signature
    mbox_settings.measure_width = settings.active_measure_width

    mbox = MeasureBox( m, g, mbox_settings )

    start_x, start_y, is_newline = measure_ind_to_xy( m_index, settings, mbox_settings, g )

    fm = FloatingMeasure( mbox, mbox_settings, start_x, start_y, is_newline )

    if is_newline:
        for i in range( 0, g.size() ):
            y = start_y + i
            x = 2
            s = g.get_string(i).open_string_note().as_string()
            if len(s) > 3:
                x = 1
            stdscr.addstr( y, x, s, noteless_color )


    selected = fm.cursor_is_in_box( cursoryx ) and settings.mode_str() == "ADD_NOTES"

    for j in range( 0, mbox.height() ):
        y = start_y + j
        for i in range( 0, mbox.width() ):
            x = start_x + i
            #print( x, y )
            #time.sleep( 10 )
            chardata = mbox.at( i, j )
            if selected:
                stdscr.addch( y, x, chardata.char, curses.color_pair(chardata.color+4) )
            else:
                stdscr.addch( y, x, chardata.char, curses.color_pair(chardata.color) )

    ############
    # draw rests
    if False:
        rests = m.compute_rests()
        for r in rests:
            x = start_x + settings.measure_buffer + int( settings.active_measure_width * r.starting_point )
            for y in range( start_y, end_y ):
                stdscr.addch( y, x, '~' )

    return fm


def draw_track( stdscr, track, cursoryx, settings: Settings ):
    maxy, maxx = stdscr.getmaxyx()
    rectangle(stdscr, 0, 0, maxy-2, maxx-1)

    topline_actions = draw_text_at_top( stdscr, track, settings )

    all_fms = []

    count = 0
    g = track.guitar
    for m in track.measures:
        all_fms.append( draw_measure( stdscr, track, count, settings, cursoryx ) )
        count += 1
    
    return all_fms, topline_actions

