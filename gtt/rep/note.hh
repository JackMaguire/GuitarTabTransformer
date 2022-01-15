#pragma once

#include <string>

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
      Gb, G, Ab, A, Bb, B
      COUNT };

  static_assert( Letter::B == 11 );
  static_assert( Letter::COUNT == 12 );
  static

  static std::string
  letter2string( Letter const l ){
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
      case( Letter::A  ): return "A";
    }
  }

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
  if( 
}


} // rep
} // gtt
