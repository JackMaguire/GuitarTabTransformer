import gtt
from gtt import *

from gtt.render.ascii import *

import json

def save_to_file( filename, track, settings ):
    track_json_str = track.serialize_to_string()
    settings_json = settings.serialize()
    settings_json[ "_track_" ] = track_json_str

    with open( filename, 'w') as fp:
        json.dump( settings_json, fp )

def load_from_file( filename, track, settings ):
    with open( filename, 'r') as fp:
        settings_json = json.load( fp )
    track.deserialize_from_string( settings_json[ "_track_" ] )
    settings.deserialize( settings_json )

def load_from_string( string, track, settings ):
    settings_json = json.loads( string )
    track.deserialize_from_string( settings_json[ "_track_" ] )
    settings.deserialize( settings_json )
    
