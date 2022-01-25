#pragma once

#include "gtt/rep/track.hh"

namespace gtt {
//namespace rep {

rep::Track
spirited_away_intro(){
  using namespace rep;

  Track t;
  t.guitar = GuitarFactory::standard_guitar();

  t.measures.resize( 4 );

  //Notes
  Note const _e = "E/4"_note;
  Note const _b = "B/3"_note;
  Note const _g = "G/3"_note;
  Note const _d = "D/3"_note;
  Note const _a = "A/2"_note;
  Note const _E = "E/2"_note;

  
  t.measures[0].set({
      //          note str   start    length
      MeasureNote( _b,  1,  1*EIGHTH, EIGHTH ),
      MeasureNote( _b,  1,  2*EIGHTH, EIGHTH ),
      MeasureNote( _b,  1,  3*EIGHTH, EIGHTH ),
      MeasureNote( _b,  1,  4*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,2,  5*EIGHTH, EIGHTH ),
      MeasureNote( _b,  1,  6*EIGHTH, EIGHTH ),
      MeasureNote( _e,  0,  7*EIGHTH, EIGHTH )
    });


  t.measures[1].set({
      //          note str       start      length
      MeasureNote( _b,  1,  0,               EIGHTH ),
      MeasureNote( _g+2,2,  EIGHTH,          EIGHTH/2.0 ),
      MeasureNote( _g+2,2,  EIGHTH+EIGHTH/2, 1.0 - (EIGHTH + EIGHTH/2.0) )
    });

  t.measures[2].set({
      //          note str   start    length
      MeasureNote( _g+2,2,  1*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,2,  2*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,2,  3*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,2,  4*EIGHTH, EIGHTH ),
      MeasureNote( _g,  2,  5*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,2,  6*EIGHTH, EIGHTH ),
      MeasureNote( _b+3,1,  7*EIGHTH, EIGHTH )
    });

  t.measures[3].set({
      //          note str   start    length
      MeasureNote( _g+2,2,  0*EIGHTH, EIGHTH ),
      MeasureNote( _g,  2,  1*EIGHTH, EIGHTH ),
      MeasureNote( _d+4,3,  2*EIGHTH, EIGHTH ),
      MeasureNote( _g  ,2,  3*EIGHTH, EIGHTH + 0.5 )
    });

}


//}//rep
}//gtt
