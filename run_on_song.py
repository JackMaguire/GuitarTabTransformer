import gtt
from gtt import *
import gtt.shortcuts as X

import curses
from curses import wrapper

import time

class Settings:
    def __init__(self):
        self.measure_buffer = 0
        self.active_measure_width = 32
        self.total_measure_width = self.active_measure_width+(2*self.measure_buffer)

        self.m_per_row = 3
        self.row_gap = 3

        self.x_gap = 5

        self.beat_size = self.active_measure_width / 4

    def x_is_beat( self, x ):
        if x < self.measure_buffer: return False

        x = x - self.measure_buffer

        if x >= self.active_measure_width: return False

        return x % self.beat_size == 0

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
    x = settings.x_gap + i_offset*(settings.total_measure_width+1)

    newline = i_offset==0

    return x, y, newline



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
            x = 1
            s = g.get_string(i).open_string_note().as_string()
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

    writer_cursor = Cursor()
    settings = Settings()

    track = Track( "example_songs/spirited_away_intro.json" )

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
        draw_track( stdscr, track, writer_cursor, settings )
        moveto( *c_pos )

        stdscr.refresh()
        k = stdscr.get_wch()

        if k in ( 'Q', 'q' ):
            y, x = stdscr.getyx()
            x -= 1
            y -= 1
            moveto( y, x )
        elif k in ( 'W', 'w' ):
            y, x = stdscr.getyx()
            y -= 1
            moveto( y, x )
        elif k in ( 'E', 'e' ):
            y, x = stdscr.getyx()
            x += 1
            y -= 1
            moveto( y, x )
        elif k in ( 'A', 'a' ):
            y, x = stdscr.getyx()
            x -= 1
            moveto( y, x )
        elif k in ( 'S', 's' ):
            pass
        elif k in ( 'D', 'd' ):
            y, x = stdscr.getyx()
            x += 1
            moveto( y, x )
        elif k in ( 'Z', 'z' ):
            y, x = stdscr.getyx()
            x -= 1
            y += 1
            moveto( y, x )
        elif k in ( 'X', 'x' ):
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
