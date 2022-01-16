#pragma once

#include <string>
#include <string_view>
#include <iostream>

#include <gtt/asserts.hh>

namespace gtt {
namespace rep {

class Note {
public:
  //using scientific pitch notation
  //https://en.wikipedia.org/wiki/Scientific_pitch_notation

  /*
    LETTERS
  */
  enum class Letter : signed char {
    C = 0, Db, D, Eb, E, F,
      Gb, G, Ab, A, Bb, B,
      COUNT };

  static_assert( int(Letter::B) == 11 );
  static_assert( int(Letter::COUNT) == 12 );

  static std::string
  letter2string( Letter const l ) ;

  static Letter
  string2letter( std::string_view const & s );

  // If you KNOW it isn't a flat or sharp
  static Letter
  char2letter( char const c );


  /*
    OCTAVES
  */
  using Octave = signed char;

  /*
    Steps
   */
  using StepType = signed char;
  constexpr static StepType HalfStep = 1;
  constexpr static StepType WholeStep = 2;
  constexpr static StepType OctaveStep = 12;

public:
  Note():
    letter_( Letter::C ),
    octave_( 4 )
  {}

  Note( Letter const l, Octave const o ):
    letter_( l ),
    octave_( o )
  {}

  Note( std::string const & notation ) {
    init_from_string( notation );
  }

  static
  void run_unit_tests();

public: //mutators  
  void init_from_string( std::string const & notation );

private:
  Letter letter_;
  Octave octave_;
};

void
Note::init_from_string( std::string const & notation ){
  //either L/# (like A/2) or LL/# (Gb/4)
  GTT_DEBUG_ASSERT( notation.size() == 3 or notation.size() == 4 );

  //find positions of dash
  auto const dash_pos = notation.find( '/' );
  GTT_ASSERT( dash_pos != std::string::npos );
  std::string_view const letter_view( &notation[0], dash_pos );
  letter_ = string2letter( letter_view );

  // assuming single char. We aren't working with <= -1 or >= 10 yet
  octave_ = notation.back() - '0';
}

std::string
Note::letter2string( Note::Letter const l ){
  using Letter = Note::Letter;
  switch( l ){
  case( Letter::A  ): return "A";
  case( Letter::Bb ): return "Bb";
  case( Letter::B  ): return "B";
  case( Letter::C  ): return "C";
  case( Letter::Db ): return "Db";
  case( Letter::D  ): return "D";
  case( Letter::Eb ): return "Eb";
  case( Letter::E  ): return "E";
  case( Letter::F  ): return "F";
  case( Letter::Gb ): return "Gb";
  case( Letter::G  ): return "G";
  case( Letter::Ab  ): return "Ab";
  default: GTT_ASSERT( false );
  }
}

Note::Letter
Note::string2letter( std::string_view const & s ){
  using Letter = Note::Letter;
  //assumes a value entry
  if( s.size() == 1 ) return Note::char2letter( s[0] );

  GTT_ASSERT_EQUALS( s.size(), 2 );
  switch( s[1] ){
  case( 'b' ):
    switch( s[0] ){
    case( 'A' ): return Letter::Ab;
    case( 'B' ): return Letter::Bb;
    case( 'D' ): return Letter::Db;
    case( 'E' ): return Letter::Eb;
    case( 'G' ): return Letter::Gb;
    default: GTT_ASSERT( false );
    }

  case ( '#' ):
    switch( s[0] ){
    case( 'G' ): return Letter::Ab;
    case( 'A' ): return Letter::Bb;
    case( 'C' ): return Letter::Db;
    case( 'D' ): return Letter::Eb;
    case( 'F' ): return Letter::Gb;
    default: GTT_ASSERT( false );
    }

  default: GTT_ASSERT( false );
  }// flat or sharp?
}

Note::Letter
Note::char2letter( char const c ){
  using Letter = Note::Letter;

  switch( c ){
  case( 'A' ): return Letter::A;
  case( 'B' ): return Letter::B;
  case( 'C' ): return Letter::C;
  case( 'D' ): return Letter::D;
  case( 'E' ): return Letter::E;
  case( 'F' ): return Letter::F;
  case( 'G' ): return Letter::G;
  default: GTT_ASSERT( false );
  }
}


void
Note::run_unit_tests(){
  {
    Note const n( "G/4" );
    GTT_ASSERT_EQUALS( int(n.letter_), int(Letter::G) );
    GTT_ASSERT_EQUALS( n.octave_, 4 );

    Note const g3 = n - Note::OctaveStep;
    GTT_ASSERT_EQUALS( int(g3.letter_), int(Letter::G) );
    GTT_ASSERT_EQUALS( g3.octave_, 3 );
  }

  {
    Note n( "Ab/0" );
    GTT_ASSERT_EQUALS( int(n.letter_), int(Letter::Ab) );
    GTT_ASSERT_EQUALS( n.octave_, 0 );

    n += 25; // A/2
    GTT_ASSERT_EQUALS( int(n.letter_), int(Letter::A) );
    GTT_ASSERT_EQUALS( n.octave_, 2 );
  }
}

} // rep
} // gtt
