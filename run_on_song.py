from gtt import *
import gtt.shortcuts as X

import curses
from curses import wrapper

def close( stdscr ):
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()

def main( stdscr ):
    # Clear screen
    stdscr.clear()
    stdscr.border(0)

    cursor_X = 0
    cursor_Y = 0
    measures_per_row = 1
    row_offset = 0

    while True:
        stdscr.refresh()
        k = stdscr.get_wch()

        if k in ( 'Q', 'q' ):
            cursor_X -= 1
            cursor_Y += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'W', 'w' ):
            cursor_Y += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'E', 'e' ):
            cursor_X += 1
            cursor_Y += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'A', 'a' ):
            cursor_X -= 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'S', 's' ):
            pass
        elif k in ( 'D', 'd' ):
            cursor_X += 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'Z', 'z' ):
            cursor_X -= 1
            cursor_Y -= 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'X', 'x' ):
            cursor_Y -= 1
            draw_game( stdscr, game, show_pocket )
        elif k in ( 'C', 'c' ):
            cursor_X += 1
            cursor_Y -= 1
            draw_game( stdscr, game, show_pocket )
    

if __name__ == '__main__':
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    stdscr.keypad(True)

    curses.start_color()
    curses.use_default_colors()
    for i in range(0, curses.COLORS):
        curses.init_pair(i + 1, curses.COLOR_BLACK, i)

    #wrapper(main)

    close( stdscr )
