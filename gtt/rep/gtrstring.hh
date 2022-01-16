#pragma once

#include "gtt/rep/note.hh"

namespace gtt {
namespace rep {

class GtrString {
public:
  GtrString(){}

  template< typename T >
  GtrString( T const & t ) :
    open_string_note_( t )
  {}

  template< typename T >
  GtrString( T const & t, signed char const max_fret ) :
    open_string_note_( t ),
    max_fret_( max_fret )
  {}

  static
  void run_unit_tests();

  signed char
  get_fret( Note const & note ) const {
    return note - open_string_note_;
  }

  bool
  can_represent( Note const & note ) const {
    signed char const fret = get_fret( note );
    return fret >= 0 and fret <= max_fret_;
  }

public: //getters/setters

  Note const &
  open_string_note() const {
    return open_string_note_;
  }

  void
  set_open_string_note( Note const & n ) {
    open_string_note_ = n;
  }

  signed char
  max_fret() const {
    return max_fret_;
  }

  void
  set_max_fret( signed char const f ) {
    max_fret_ = f;
  }

private:
  Note open_string_note_;
  signed char max_fret_ = 24;
};

void
GtrString::run_unit_tests(){
  GtrString s( "Ab/5", 24 );
  GTT_ASSERT_EQUALS( s.get_fret( s.open_string_note() ), 0 );
  for( int i = -20; i <= 20; ++i ){
    GTT_ASSERT_EQUALS( s.get_fret( s.open_string_note()+i ), i );
  }

  GTT_ASSERT( not s.can_represent( Note( "Ab/4" ) ) );
  GTT_ASSERT( not s.can_represent( Note( "Ab/5" )-1 ) );
  GTT_ASSERT( s.can_represent( Note( "Ab/5" ) ) );
  GTT_ASSERT( s.can_represent( Note( "Ab/6" ) ) );
  GTT_ASSERT( s.can_represent( Note( "Ab/7" )-1 ) );
  GTT_ASSERT( s.can_represent( Note( "Ab/7" ) ) );
  GTT_ASSERT( not s.can_represent( Note( "Ab/7" )+1 ) );
  GTT_ASSERT( not s.can_represent( Note( "Ab/8" ) ) );

  s.set_max_fret( 25 );
  GTT_ASSERT( s.can_represent( Note( "Ab/7" )+1 ) );
  GTT_ASSERT( not s.can_represent( Note( "Ab/8" ) ) );
}

} // rep
} // gtt
