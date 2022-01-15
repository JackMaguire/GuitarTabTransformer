#pragma once

#include <string>

namespace gtt {
namespace rep {

class Note {
public:

  /*
    LETTERS
   */
  enum class Letter : unsigned char {
    A = 0, Bb, B, C, Db, D,
      Eb, E, F, Gb, G, Ab,
      COUNT };

  static_assert( Letter::Ab == 11 );
  static_assert( Letter::COUNT == 12 );

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
  Note( Letter const l, Octave const o ):
    letter_( l ),
    octave_( o )
  {}

private:
  Letter letter_;
  Octave octave_;
};

} // rep
} // gtt
