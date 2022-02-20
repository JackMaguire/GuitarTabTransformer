import gtt
from gtt import *
import gtt.shortcuts as X

import curses
from curses import wrapper
from curses.textpad import Textbox, rectangle

import time

from py.settings import Settings

class Cursor:
    def __init__(self):
        self.x = 0
        self.y = 0

def close( stdscr ):
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

def make_edit_window( stdscr ):
    editwin = curses.newwin(5,30, 2,1)
    rectangle(stdscr, 1,0, 1+5+1, 1+30+1)
    stdscr.refresh()

    box = Textbox(editwin)

    # Let the user edit until Ctrl-G is struck.
    box.edit()

    # Get resulting contents
    message = box.gather()

    del editwin
    stdscr.touchwin()

    return message

def measure_ind_to_xy( index, settings: Settings, g: gtt.Guitar ):
    j_offset = int( index / settings.m_per_row )
    y = 3 + j_offset*(settings.row_gap+g.size())

    i_offset = index % settings.m_per_row
    x = settings.x_gap + i_offset*(settings.total_measure_width+1)

    newline = i_offset==0

    return x, y, newline


def draw_text_at_top(
        stdscr,
        track: gtt.Track,
        settings: Settings
):
    text_color = curses.color_pair(242)
    
    y = 1
    x = settings.x_gap

    # Time Signature
    ts_top_str = str(track.time_signature.top)
    stdscr.addstr( y, x, ts_top_str, text_color )
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

def draw_measure(
        stdscr,
        m: gtt.Measure,
        g: gtt.Guitar,
        #cursor: Cursor,
        m_ind: int,
        settings: Settings
):
    noteless_color = curses.color_pair(242)

    #################
    # draw background
    start_x, start_y, is_newline = measure_ind_to_xy( m_ind, settings, g )
    #print( m_ind, start_x, start_y )
    end_x = start_x + settings.total_measure_width
    end_y = start_y + g.size()
    for x in range( start_x, end_x ):
        if settings.x_is_beat( x - start_x ):
            color = curses.color_pair(252)
        else:
            color = noteless_color

        for y in range( start_y, end_y ):
            stdscr.addch( y, x, '-', color )

    if is_newline:
        for i in range( 0, g.size() ):
            y = start_y + i
            x = 2
            s = g.get_string(i).open_string_note().as_string()
            if len(s) > 3:
                x = 1
            stdscr.addstr( y, x, s, noteless_color )

    ############
    # draw notes
    for i in range( 0, len(m) ):
        note = m[i]
        y = start_y + note.string_assignment
        x = start_x + settings.measure_buffer + int( settings.active_measure_width * note.starting_point )
        fret = g.get_string( note.string_assignment ).get_fret( note.note )
        stdscr.addstr( y, x, str(fret) )


    ############
    # draw rests
    if False:
        rests = m.compute_rests()
        for r in rests:
            x = start_x + settings.measure_buffer + int( settings.active_measure_width * r.starting_point )
            for y in range( start_y, end_y ):
                stdscr.addch( y, x, '~' )

def draw_track( stdscr, track, cursor: Cursor, settings: Settings ):
    maxy, maxx = stdscr.getmaxyx()
    rectangle(stdscr, 0, 0, maxy-2, maxx-1)

    draw_text_at_top( stdscr, track, settings )

    count = 0
    g = track.guitar
    for m in track.measures:
        #print( count, m )
        draw_measure( stdscr, m, g, count, settings )
        count += 1

def main( stdscr ):
    # Clear screen
    stdscr.clear()
    stdscr.border(0)

    track = Track( "example_songs/spirited_away_intro.json" )

    writer_cursor = Cursor()
    settings = Settings( track )

    #draw_track( stdscr, track, writer_cursor, settings )
    #stdscr.refresh()
    #time.sleep( 10 )

    def moveto( y, x ):
        maxy, maxx = stdscr.getmaxyx()

        if y > maxy-1: y = maxy-1
        elif y < 1: y = 1

        if x > maxx-1: x = maxx-1
        elif x < 1: x = 1

        stdscr.move( y, x )


    while True:
        c_pos = stdscr.getyx()
        stdscr.clear()
        draw_track( stdscr, track, writer_cursor, settings )
        moveto( *c_pos )

        stdscr.refresh()
        k = stdscr.get_wch()

        #print( k )
        #time.sleep( 10 )

        down = 258
        up = 259
        left = 260
        right = 261

        if k in ( 'Q', 'q' ):
            y, x = stdscr.getyx()
            x -= 1
            y -= 1
            moveto( y, x )
        elif k in ( 'W', 'w', up ):
            y, x = stdscr.getyx()
            y -= 1
            moveto( y, x )
        elif k in ( 'E', 'e' ):
            y, x = stdscr.getyx()
            x += 1
            y -= 1
            moveto( y, x )
        elif k in ( 'A', 'a', left ):
            y, x = stdscr.getyx()
            x -= 1
            moveto( y, x )
        elif k in ( 'S', 's' ):
            #s = make_edit_window( stdscr )
            #x = int( s )
            #moveto( 10, x )
            pass
        elif k in ( 'D', 'd', right ):
            y, x = stdscr.getyx()
            x += 1
            moveto( y, x )
        elif k in ( 'Z', 'z' ):
            y, x = stdscr.getyx()
            x -= 1
            y += 1
            moveto( y, x )
        elif k in ( 'X', 'x', down ):
            y, x = stdscr.getyx()
            y += 1
            moveto( y, x )
        elif k in ( 'C', 'c' ):
            y, x = stdscr.getyx()
            x += 1
            y += 1
            moveto( y, x )


if __name__ == '__main__':
    #t = Track( "example_songs/spirited_away_intro.json" )
    #print( t )

    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    stdscr.keypad(True)

    curses.start_color()
    curses.use_default_colors()
    for i in range(0, curses.COLORS):
        #curses.init_pair(i + 1, curses.COLOR_BLACK, i)
        curses.init_pair(i + 1, i, -1)

    wrapper(main)

    close( stdscr )
