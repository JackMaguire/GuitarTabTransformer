import gtt
from gtt import *
import gtt.shortcuts as X

import curses
from curses import wrapper

import time

class Settings:
    def __init__(self):
        self.measure_buffer = 2
        self.active_measure_width = 32
        self.total_measure_width = self.active_measure_width+(2*self.measure_buffer)

        self.m_per_row = 3
        self.row_gap = 3

class Cursor:
    def __init__(self):
        self.x = 0
        self.y = 0

def close( stdscr ):
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

def measure_ind_to_xy( index, settings: Settings, g: gtt.Guitar ):
    j_offset = int( index / settings.m_per_row )
    y = 3 + j_offset*(settings.row_gap+g.size())

    i_offset = index % settings.m_per_row
    x = 3 + i_offset*(settings.total_measure_width+1)

    return x, y

    

def draw_measure(
        stdscr,
        m: gtt.Measure,
        g: gtt.Guitar,
        #cursor: Cursor,
        m_ind: int,
        settings: Settings
):
    #################
    # draw background

    start_x, start_y = measure_ind_to_xy( m_ind, settings, g )
    #print( m_ind, start_x, start_y )
    end_x = start_x + settings.total_measure_width
    end_y = start_y + g.size()
    for x in range( start_x, end_x ):
        for y in range( start_y, end_y ):
            stdscr.addch( y, x, '-', curses.color_pair(246) )

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
    rests = m.compute_rests()
    for r in rests:
        #y = start_y + note.string_assignment
        x = start_x + settings.measure_buffer + int( settings.active_measure_width * r.starting_point )
        #fret = g.get_string( note.string_assignment ).get_fret( note.note )
        #stdscr.addstr( y, x, str(fret) )

def draw_track( stdscr, track, cursor: Cursor, settings: Settings ):
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

    user_cursor = Cursor()

    writer_cursor = Cursor()
    settings = Settings()

    track = Track( "example_songs/spirited_away_intro.json" )

    draw_track( stdscr, track, writer_cursor, settings )
    #stdscr.refresh()
    time.sleep( 10 )

    while True:
        stdscr.refresh()
        k = stdscr.get_wch()

        if k in ( 'Q', 'q' ):
            user_cursor.x -= 1
            user_cursor.y += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'W', 'w' ):
            user_cursor.y += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'E', 'e' ):
            user_cursor.x += 1
            user_cursor.y += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'A', 'a' ):
            user_cursor.x -= 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'S', 's' ):
            pass
        elif k in ( 'D', 'd' ):
            user_cursor.x += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'Z', 'z' ):
            user_cursor.x -= 1
            user_cursor.y -= 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'X', 'x' ):
            user_cursor.y -= 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'C', 'c' ):
            user_cursor.x += 1
            user_cursor.y -= 1
            draw_game( stdscr, game, show_pocket )
    

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
