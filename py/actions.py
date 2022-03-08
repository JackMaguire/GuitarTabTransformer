import gtt
from gtt import *

from py.settings import Settings
from py.saveload import save_to_file, load_from_str

import time

class ActionMap:
    def __init__( self, stdscr ):
        maxy, maxx = stdscr.getmaxyx()
        # index by (y, x)
        self.actions = [ 
            [ None for _ in range( 0, maxx ) ] for _ in range( 0, maxy )
        ]

    def set_action( self, y, x, action ):
        self.actions[ y ][ x ] = action

    def get_action( self, y, x ):
        return self.actions[ y ][ x ]

class Action:
    def __init__( self ):
        pass

    def handle_string_entry( self, track, settings, setting_str ):
        pass

    def handle_increment( self, track, settings ):
        pass

    def handle_decrement( self, track, settings ):
        pass

    def handle_enter( self, track, settings ):
        pass

    def handle_delete( self, track, settings ):
        pass


class StringChanger( Action ):
    def __init__( self, str_no ):
        self.str_no = str_no

    def handle_string_entry( self, track, settings, setting_str ):
        try:
            note = Note( setting_str )
            track.guitar.set_string_note( self.str_no, note )
            return
        except Exception as e:
            pass

    def handle_increment( self, track, settings ):
        try:
            note = track.guitar[ self.str_no ].open_string_note() + 1
            track.guitar.set_string_note( self.str_no, note )
        except Exception as e:
            print( e )
            time.sleep( 3 )

    def handle_decrement( self, track, settings ):
        try:
            note = track.guitar[ self.str_no ].open_string_note() - 1
            track.guitar.set_string_note( self.str_no, note )
        except Exception as e:
            print( e )
            time.sleep( 3 )
    

class UpdateSigTop( Action ):
    def handle_string_entry( self, track, settings, setting_str ):
        try:
            new_top = int( setting_str.strip().rstrip() )
            track.time_signature.top = new_top
        except:
            pass

    def handle_increment( self, track, settings ):
        track.time_signature.top += 1

    def handle_decrement( self, track, settings ):
        if track.time_signature.top > 1:
            track.time_signature.top -= 1

class UpdateMWidth( Action ):
    def handle_string_entry( self, track, settings, setting_str ):
        try:
            new_mw = int( setting_str.strip().rstrip() )
            settings.active_measure_width = new_mw
        except:
            pass

    def handle_increment( self, track, settings ):
        settings.active_measure_width += 1

    def handle_decrement( self, track, settings ):
        if settings.active_measure_width > 4:
            settings.active_measure_width -= 1

class SaveJsonPlease( Action ):
    def handle_string_entry( self, track, settings, setting_str ):
        try:
            filename = setting_str.strip().rstrip()
            save_data = save_to_file( filename, track, settings )
        except Exception as e:
            try:
                save_to_file( "/tmp/gtt_save.json", track, settings )
                print( "saved to /tmp/gtt_save.json because of error:", e )
            except:
                print( "No save!!!", e )

class ExistingNoteAction( Action ):
    def __init__( self, measure_ind, note_ind ):
        self.measure_ind = measure_ind
        self.note_ind = note_ind

    def handle_string_entry( self, track, settings, setting_str ):
        pass

    def handle_increment( self, track, settings ):
        if settings.mode_str() == "EDIT":
            curr_ass = track.measures[ self.measure_ind ][ self.note_ind ].string_assignment
            if curr_ass+1 < track.guitar.size():
                track.measures[ self.measure_ind ].change_string_assignment( self.note_ind, curr_ass+1 )
                settings.move_cursor( 1 )


    def handle_decrement( self, track, settings ):
        if settings.mode_str() == "EDIT":
            curr_ass = track.measures[ self.measure_ind ][ self.note_ind ].string_assignment
            if curr_ass > 0:
                track.measures[ self.measure_ind ].change_string_assignment( self.note_ind, curr_ass-1 )
                settings.move_cursor( -1 )

    def handle_enter( self, track, settings ):
        pass

    def handle_delete( self, track, settings ):
        if settings.mode_str() == "ADD_NOTES":
            track.measures[ self.measure_ind ].remove( self.note_ind )
