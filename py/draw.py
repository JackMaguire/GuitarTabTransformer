import gtt
from gtt import *
from gtt.render.ascii import *

from py.settings import Settings
from py.actions import *

import curses
#from curses.textpad import rectangle

def add_over_range( local_actions, y, x, string, action ):
     for i in range( 0, len( string ) ):
         local_actions.set_action( y=y, x=x+i, action=action )

def draw_text_at_top(
        stdscr,
        track: gtt.Track,
        settings: Settings,
        local_actions
):
    text_color = curses.color_pair(242)
    
    y = 1
    x = settings.x_gap

    def add_action( x, string, action ):
        add_over_range( local_actions, y, x, string, action )

    # Time Signature
    ts_top_str = str(track.time_signature.top)
    stdscr.addstr( y, x, ts_top_str, text_color )
    add_action( x, ts_top_str, UpdateSigTop() )
    x += len(ts_top_str)

    stdscr.addch( y, x, '/', text_color )
    x += 1

    ts_bottom_str = str(track.time_signature.bottom)
    stdscr.addstr( y, x, ts_bottom_str, text_color )
    x += len(ts_bottom_str) + 2

    # Key
    key_str = track.key_str()
    stdscr.addstr( y, x, key_str, text_color )
    add_action( x, key_str, KeySigAnnotation() )
    x += len(key_str) + 4

    # Width
    w_str = "WIDTH: "
    stdscr.addstr( y, x, w_str, text_color )
    x += len(w_str)
    w_str = str( settings.active_measure_width )
    stdscr.addstr( y, x, w_str, text_color )
    add_action( x, w_str, UpdateMWidth() )
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
    add_action( x, next_str, SaveJsonPlease() )
    x += len( next_str ) + 2


    # NEXT LINE
    y+= 1
    x = settings.x_gap

    # "MIN/MAX_FRET"
    next_str = "MIN_FRET"
    stdscr.addstr( y, x, next_str, curses.color_pair(3) )
    add_action( x, next_str, MinFret() )
    x += len( next_str ) + 2

    next_str = "MAX_FRET"
    stdscr.addstr( y, x, next_str, curses.color_pair(4) )
    add_action( x, next_str, MaxFret() )
    x += len( next_str ) + 4
    

def calc_m_per_row( measure_width, maxx ):
    return int( (maxx-6) / (measure_width+1) )

def measure_ind_to_xy( index, settings: Settings, mbox_settings, g: gtt.Guitar, maxx ):
    m_per_row = calc_m_per_row( mbox_settings.measure_width, maxx )

    j_offset = int( index / m_per_row )
    y = 3 + j_offset*(settings.row_gap+g.size())

    i_offset = index % m_per_row
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
        cursoryx,
        local_actions,
        spacial_m_position
):
    noteless_color = curses.color_pair(242)

    m: gtt.Measure = t.measures[ m_index ]
    g: gtt.Guitar =  t.guitar

    mbox_settings = MeasureBoxSettings()
    mbox_settings.time_sig = t.time_signature
    mbox_settings.measure_width = settings.active_measure_width

    mbox = MeasureBox( m, g, mbox_settings )
    maxy, maxx = stdscr.getmaxyx()

    start_x, start_y, is_newline = measure_ind_to_xy( spacial_m_position, settings, mbox_settings, g, maxx )

    stop_y = start_y + mbox.height()
    if stop_y > maxy:
        return

    fm = FloatingMeasure( mbox, mbox_settings, start_x, start_y, is_newline )

    if is_newline:
        for i in range( 0, g.size() ):
            y = start_y + i + 1
            x = 2
            s = g.get_string(i).open_string_note().as_string()
            if len(s) > 3:
                x = 1
            stdscr.addstr( y, x, s, noteless_color )
            add_over_range( local_actions, y, x, s, StringChanger(i) )


    #selected = fm.cursor_is_in_box( cursoryx ) and settings.mode_str()=="ADD_NOTES"

    for j in range( 0, mbox.height() ):
        y = start_y + j
        for i in range( 0, mbox.width() ):
            x = start_x + i
            chardata = mbox.at( i, j )
            stdscr.addch( y, x, chardata.char, curses.color_pair(chardata.color) )
            if chardata.note_index >= 0:
                 local_actions.set_action( y=y, x=x, action=ExistingNoteAction( m_index, chardata.note_index ) )
            else:
                 local_actions.set_action( y=y, x=x, action=EditMeasure( m_index ) )

    # annotation actions
    for i in range( 0, mbox.width() ):
        ann_index = mbox.annotation_index_for_cell( i );
        if ann_index >= 0:
            local_actions.set_action( y=start_y, x=start_x+i, action=EditAnnotation( m_index, ann_index ) )
        else:
            starting_point = float(i) / float(mbox.width())
            if i+1 == mbox.width():
                starting_point = 0.99
            local_actions.set_action( y=start_y, x=start_x+i, action=AddAnnotation( m_index, starting_point ) )
            

    return fm


def draw_track( stdscr, track, cursoryx, settings: Settings ):
    maxy, maxx = stdscr.getmaxyx()
    stdscr.border(0)
    #rectangle(stdscr, 0, 0, maxy-2, maxx-1)

    local_actions = ActionMap( stdscr )

    draw_text_at_top( stdscr, track, settings, local_actions )

    all_fms = []

    n_measures_skipped = settings.n_rows_skipped * calc_m_per_row( settings.active_measure_width, maxx )

    count = 0
    g = track.guitar
    for m in track.measures:
        if count < n_measures_skipped:
            count += 1
            continue

        fm = draw_measure( stdscr, track, count, settings, cursoryx, local_actions, count-n_measures_skipped )
        if fm != None:
            all_fms.append( fm ) 
        count += 1
    
    return all_fms, local_actions

