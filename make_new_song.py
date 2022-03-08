import gtt
from gtt import *
import gtt.shortcuts as X
from gtt.render.ascii import StateCache

import time

from py.settings import Settings
from py.saveload import save_to_file

import argparse


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('--file', type=str, required=True )
    parser.add_argument('--nmeasures', type=int, required=True )
    parser.add_argument('--guitar', type=str, default="6str", choices=["6str","7str","4bass","5bass"] )
    args = parser.parse_args()

    track = Track( args.nmeasures, args.guitar )
    settings = Settings( track, None )

    save_to_file( args.file, track, settings )
