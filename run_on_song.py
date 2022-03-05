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

def enter_is_terminate(x):
    if x == 10:
        return 7
    return x

def make_edit_window( stdscr ):
    editwin = curses.newwin(5,30, 2,1)
    rectangle(stdscr, 1,0, 1+5+1, 1+30+1)
    stdscr.refresh()

    box = Textbox(editwin)
    box.edit(enter_is_terminate)

    # Let the user edit until Ctrl-G is struck.
    #box.edit()

    # Get resulting contents
    message = box.gather()

    del editwin
    stdscr.touchwin()

    return message

def handle_move( k, stdscr, moveto ):
    down = 258
    up = 259
    left = 260
    right = 261

    if k in ( 'W', 'w', up ):
        y, x = stdscr.getyx()
        y -= 1
        moveto( y, x )
    elif k in ( 'A', 'a', left ):
        y, x = stdscr.getyx()
        x -= 1
        moveto( y, x )
    elif k in ( 'S', 's', down ):
        y, x = stdscr.getyx()
        y += 1
        moveto( y, x )
    elif k in ( 'D', 'd', right ):
        y, x = stdscr.getyx()
        x += 1
        moveto( y, x )

def char_to_fret( k, stdscr ):
    if k == '1': return 1
    if k == '2': return 2
    if k == '3': return 3
    if k == '4': return 4
    if k == '5': return 5
    if k == '6': return 6
    if k == '7': return 7
    if k == '8': return 8
    if k == '9': return 9
    if k in '`0': return 0

    if k == '!': return 11
    if k == '@': return 12
    if k == '#': return 13
    if k == '$': return 14
    if k == '%': return 15
    if k == '^': return 16
    if k == '&': return 17
    if k == '*': return 18
    if k == '(': return 19
    if k in ')~': return 10

    k_str = make_edit_window( stdscr ).strip().rstrip()
    try:
        i = int(k_str)
        return i
    except:
        return -1

def handle_new_note( k, stdscr, floating_measures, track ):
    cursoryx = stdscr.getyx()
    y, x = cursoryx
    fm = [ f for f in floating_measures if f.cursor_is_in_box( cursoryx ) ]
    if len( fm ) == 0:
        return
    else:
        fm = fm[0]

    str_index = y - fm.start_y
    #g = track.guitar
    s = track.guitar.get_string( str_index )
    fret = char_to_fret( k, stdscr )
    if fret < 0: return

    note_int = s.open_string_note().as_int() + fret

    start_point = float( x - fm.start_x ) / fm.mbox.width()
    length = 0 # TODO?

    mn = MeasureNote( note_int, start_point, length )
    mn.string_assignment = str_index
    fm.mbox.add_note( mn )
    #print( fret )
    #time.sleep( 10 )

def handle_topline_action( stdscr, topline_actions, track, settings, x ):
    if x in topline_actions:
        setting = make_edit_window( stdscr ).strip().rstrip()
        topline_actions[x]( track=track, settings=settings, setting_str=setting )    

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
        cursoryx = stdscr.getyx()
        stdscr.clear()
        floating_measures, topline_actions = draw_track( stdscr, track, cursoryx, settings )
        moveto( *cursoryx )

        stdscr.refresh()
        k = stdscr.get_wch()
        strk = str(k)

        #print( k )
        #time.sleep( 10 )

        y, x = stdscr.getyx()

        down = 258
        up = 259
        left = 260
        right = 261

        # Cursor Movement:
        if k in ( ' ', ):
            settings.toggle_mode()
        elif k in ( left, right, up, down ) or strk in 'WASDwasd':
            handle_move( k, stdscr, moveto )
        elif settings.mode_str() == "ADD_NOTES" and strk in '1234567890!@#$%^&*()`~+':
            handle_new_note( k, stdscr, floating_measures, track )
            moveto( *cursoryx )
        elif settings.mode_str() == "EDIT" and strk in '+\n' and y == 1:
            handle_topline_action( stdscr, topline_actions, track, settings, x )
            #print( 123 )
            #time.sleep( 10 )
            


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
