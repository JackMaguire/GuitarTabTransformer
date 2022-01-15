#pragma once

#include <string>

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
  enum class Letter : unsigned char {
    C = 0, Db, D, Eb, E, F,
      Gb, G, Ab, A, Bb, B,
      COUNT };

  static_assert( int(Letter::B) == 11 );
  static_assert( int(Letter::COUNT) == 12 );

  static std::string
  letter2string( Letter const l ) ;

  static Letter
  string2letter( std::string const & s );


  /*
    OCTAVES
  */
  using Octave = unsigned char;

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

public: //mutators  
  void init_from_string( std::string const & notation );

private:
  Letter letter_;
  Octave octave_;
};

void
Note::init_from_string( std::string const & notation ){
  //either L/# (like A/2) or LL/# (Gb/4)

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
Note::string2letter( std::string const & s ){
  using Letter = Note::Letter;
  //assumes a value entry
  switch( s.size() ){
  case( 1 ):
    switch( s[0] ){
    case( 'A' ): return Letter::A;
    case( 'B' ): return Letter::B;
    case( 'C' ): return Letter::C;
    case( 'D' ): return Letter::D;
    case( 'E' ): return Letter::E;
    case( 'F' ): return Letter::F;
    case( 'G' ): return Letter::G;
    default: GTT_ASSERT( false );
    }
    break;

  case( 2 ):
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
    break;

  default: GTT_ASSERT( false );
  } //how big?

}


} // rep
} // gtt
