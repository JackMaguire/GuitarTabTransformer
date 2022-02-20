import gtt
from gtt import *
import gtt.shortcuts as X

import curses
from curses import wrapper
from curses.textpad import Textbox, rectangle

import time

from py.settings import Settings
from py.draw import draw_track

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
