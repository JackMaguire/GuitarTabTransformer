import gtt
from gtt import *
import gtt.shortcuts as X
from gtt.render.ascii import StateCache

import curses
from curses import wrapper
from curses.textpad import Textbox, rectangle

import time

from py.settings import Settings
from py.draw import draw_track
from py.saveload import load_from_file, save_to_str, load_from_str
from py.colors import *

import argparse

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

    # Let the user edit until Ctrl-G or Enter is struck.
    box.edit(enter_is_terminate)

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

    str_index = y - fm.start_y - 1 #-1 for annotation line
    s = track.guitar.get_string( str_index )

    fret = char_to_fret( k, stdscr )
    if fret < 0: return

    note_int = s.open_string_note().as_int() + fret

    start_point = float( x - fm.start_x ) / fm.mbox.width()
    length = 0 # TODO?

    mn = MeasureNote( note_int, start_point, length )
    mn.string_assignment = str_index
    fm.mbox.add_note( mn )

def prompt_for_local_action( stdscr, local_actions, track, settings ):
    y, x = stdscr.getyx()
    action = local_actions.get_action( y=y, x=x )
    if action == None: return

    settings_str = make_edit_window( stdscr ).strip().rstrip()
    action.handle_string_entry( track=track, settings=settings, setting_str=settings_str )

def main( stdscr ):
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('--load', type=str, required=False)
    args = parser.parse_args()

    # Clear screen
    stdscr.clear()
    stdscr.border(0)

    if args.load:
        track = Track()
        settings = Settings( track, stdscr )
        load_from_file( args.load, track, settings )
    else:
        track = Track( "example_songs/spirited_away_intro.json" )
        settings = Settings( track, stdscr )
        
    def moveto( y, x ):
        maxy, maxx = stdscr.getmaxyx()

        if y > maxy-1: y = maxy-1
        elif y < 1: y = 1

        if x > maxx-1: x = maxx-1
        elif x < 1: x = 1

        stdscr.move( y, x )

    sc = StateCache( save_to_str( track, settings ) )

    while True:
        cursoryx = stdscr.getyx()
        stdscr.clear()
        floating_measures, local_actions = draw_track( stdscr, track, cursoryx, settings )
        moveto( *cursoryx )

        stdscr.refresh()
        k = stdscr.get_wch()
        strk = str(k)
           
        #print( k )
        #time.sleep( 10 )

        down = 258
        up = 259
        left = 260
        right = 261

        while k in ( left, right, up, down ) or strk in 'WASDwasd':
            handle_move( k, stdscr, moveto ) 
            k = stdscr.get_wch()
            strk = str(k)

        y, x = stdscr.getyx()

        # Cursor Movement:
        if k in ( ' ', ):
            settings.toggle_mode()
            continue # no save
        elif k in ( left, right, up, down ) or strk in 'WASDwasd':
            handle_move( k, stdscr, moveto )
            continue # no save
        elif settings.mode_str() == "VIEW":
            continue # no save
        elif settings.mode_str() == "ADD_NOTES" and strk in '1234567890!@#$%^&*()`~+':
            handle_new_note( k, stdscr, floating_measures, track )
            moveto( y, x )
        elif strk == '+':
            prompt_for_local_action( stdscr, local_actions, track, settings )
        elif strk in '{}':
            action = local_actions.get_action( y=y, x=x )
            if action != None:
                if strk == '{': action.handle_decrement( track, settings )
                else: action.handle_increment( track, settings )
        elif strk == '\n':
            action = local_actions.get_action( y=y, x=x )
            if action != None: action.handle_enter( track, settings )
        elif k == curses.KEY_BACKSPACE:
            action = local_actions.get_action( y=y, x=x )
            if action != None: action.handle_delete( track, settings )
        elif strk == 'U':
            load_from_str( sc.undo(), track, settings )
        elif strk == 'R':
            load_from_str( sc.redo(), track, settings )
        elif strk == '>':
            settings.n_rows_skipped += 1
        elif strk == '<':
            settings.n_rows_skipped = max( 0, settings.n_rows_skipped - 1 )
        else:
            continue # no save

        sc.register_new_state( save_to_str( track, settings ) )

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
