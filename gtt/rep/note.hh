#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <concepts>
#include <compare>

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
    STEPS
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

  Note( signed char const note ) {
    init_from_int( note );
  }

  explicit Note( int const note ) {
    init_from_int( note );
  }

  Note( Note const & ) = default;

  static
  void run_unit_tests();

public:
  void init_from_string( std::string const & notation );

  std::string
  as_string() const {
    //either L/# (like A/2) or LL/# (Gb/4)
    return letter2string( letter_ ) + '/' + char('0' + octave_);
  }

  void init_from_int( signed char const i ){
    constexpr signed char n_notes_per_octave = 12;
    octave_ = i / n_notes_per_octave;
    letter_ = Letter( i % n_notes_per_octave );
  }

  signed char
  as_int() const {
    constexpr signed char n_notes_per_octave = 12;
    return n_notes_per_octave*int(octave_) + int(letter_);
  }

public: //operators

  operator signed char() const{
    return as_int();
  }

  template< typename T >
  Note
  operator-( T const diff ) const {
    return Note( as_int() - static_cast<signed char>(diff) );
  }

  Note &
  operator-=( signed char const diff ) {
    init_from_int( as_int() - diff );
    return *this;
  }

  template< typename T >
  Note
  operator+( T const diff ) const {
    return Note( as_int() + diff );
  }

  Note &
  operator+=( signed char const diff ) {
    init_from_int( as_int() + diff );
    return *this;
  }

  std::strong_ordering
  operator<=>( Note const & that) const {
    //https://en.cppreference.com/w/cpp/language/default_comparisons
    if (auto cmp = octave_ <=> that.octave_; cmp != 0){
      return cmp;
    } else {
      return letter_ <=> that.letter_;
    }
  }

  bool
  operator==( Note const & other ) const {
    return letter_ == other.letter_ and octave_ == other.octave_;
  }

public:
  Letter letter() const{ return letter_; }
  Octave octave() const{ return octave_; }

private:
  Letter letter_;
  Octave octave_;
};

Note
operator "" _note( char const * a, unsigned long const u ){
  std::string const s( a, u );
  return Note( s );
}

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
    //test construct
    Note const n( "G/4" );
    GTT_ASSERT_EQUALS( int(n.letter_), int(Letter::G) );
    GTT_ASSERT_EQUALS( n.octave_, 4 );

    //test ints
    int const n_as_int = n.as_int();
    Note const n_from_int( n_as_int );
    GTT_ASSERT( n_from_int == n );

    //test mutate operator
    Note const g3 = n - Note::OctaveStep;
    GTT_ASSERT_EQUALS( int(g3.letter_), int(Letter::G) );
    GTT_ASSERT_EQUALS( g3.octave_, 3 );
  }

  {
    Note n( "Ab/0" );
    GTT_ASSERT_EQUALS( int(n.letter_), int(Letter::Ab) );
    GTT_ASSERT_EQUALS( n.octave_, 0 );
    GTT_ASSERT( n == "Ab/0"_note );

    n += 25; // A/2
    GTT_ASSERT_EQUALS( int(n.letter_), int(Letter::A) );
    GTT_ASSERT_EQUALS( n.octave_, 2 );
  }

  {
    GTT_ASSERT( Note( "A/1" ) != Note( "B/1" ) );
    GTT_ASSERT( Note( "A/1" ) <  Note( "B/1" ) );
    GTT_ASSERT( Note( "A/1" ) <= Note( "A/1" ) );
    GTT_ASSERT( Note( "A/1" ) == Note( "A/1" ) );
    GTT_ASSERT( Note( "A/1" ) >= Note( "A/1" ) );
    GTT_ASSERT( Note( "G/5" ) >  Note( "A/4" ) );
    GTT_ASSERT( Note( "G/5" ) != Note( "A/4" ) );
  }
}

} // rep
} // gtt
