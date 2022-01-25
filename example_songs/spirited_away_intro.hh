#pragma once

#include "gtt/rep/track.hh"

namespace gtt {
//namespace rep {

rep::Track
spirited_away_intro(){
  using namespace rep;

  Track t;
  t.guitar = GuitarFactory::standard_guitar();
  Guitar const & gtr = t.guitar;

  t.measures.resize( 4 );

  //Notes
  [[maybe_unused]] Note const _e = "E/4"_note;
  [[maybe_unused]] Note const _b = "B/3"_note;
  [[maybe_unused]] Note const _g = "G/3"_note;
  [[maybe_unused]] Note const _d = "D/3"_note;
  [[maybe_unused]] Note const _a = "A/2"_note;
  [[maybe_unused]] Note const _E = "E/2"_note;

  
  t.measures[0].set({
      //          note str   start    length
      MeasureNote( _b,  gtr,  1*EIGHTH, EIGHTH ),
      MeasureNote( _b,  gtr,  2*EIGHTH, EIGHTH ),
      MeasureNote( _b,  gtr,  3*EIGHTH, EIGHTH ),
      MeasureNote( _b,  gtr,  4*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,gtr,  5*EIGHTH, EIGHTH ),
      MeasureNote( _b,  gtr,  6*EIGHTH, EIGHTH ),
      MeasureNote( _e,  gtr,  7*EIGHTH, EIGHTH )
    });


  t.measures[1].set({
      //          note str       start      length
      MeasureNote( _b,  gtr,  0,               EIGHTH ),
      MeasureNote( _g+2,gtr,  EIGHTH,          EIGHTH/2.0 ),
      MeasureNote( _g+2,gtr,  EIGHTH+EIGHTH/2, 1.0 - (EIGHTH + EIGHTH/2.0) )
    });

  t.measures[2].set({
      //          note str   start    length
      MeasureNote( _g+2,gtr,  1*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,gtr,  2*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,gtr,  3*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,gtr,  4*EIGHTH, EIGHTH ),
      MeasureNote( _g,  gtr,  5*EIGHTH, EIGHTH ),
      MeasureNote( _g+2,gtr,  6*EIGHTH, EIGHTH ),
      MeasureNote( _b+3,gtr,  7*EIGHTH, EIGHTH )
    });

  t.measures[3].set({
      //          note str   start    length
      MeasureNote( _g+2,gtr,  0*EIGHTH, EIGHTH ),
      MeasureNote( _g,  gtr,  1*EIGHTH, EIGHTH ),
      MeasureNote( _d+4,gtr,  2*EIGHTH, EIGHTH ),
      MeasureNote( _g  ,gtr,  3*EIGHTH, EIGHTH + 0.5 )
    });

  return t;
}


//}//rep
}//gtt
