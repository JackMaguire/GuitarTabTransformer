#pragma once

#include "gtt/rep/note.hh"
#include <string>
#include <vector>
#include <set>
#include <map>

namespace gtt {
namespace rep {

/*template< typename T >
constexpr
T div3( T const t ){
  static_assert( t % 3 == 0 );
  return t / 3;
}

struct NoteDuration {
  using DurationInt = signed char;
  // currently measuring everything in 24th notes to accomodate triplets

  constexpr static DurationInt whole_note = 24;
  constexpr static DurationInt half_note = whole_note/2;
  constexpr static DurationInt quarter_note = whole_note/4;
  constexpr static DurationInt eighth_note = whole_note/8;

  //triplet such that all 3 elements fit in the span of a quarter note
  constexpr static DurationInt quarter_triplet = div3(quarter_note);

  //triplet such that all 3 elements fit in the span of a half note
  constexpr static DurationInt half_triplet = div3(half_note);
};*/

/*enum class Duration : signed char {
  WHOLE,
    HALF,
    QUARTER,
    EIGHTH,

  //triplet such that all 3 elements fit in the span of a quarter note
    QUARTER_TRIP,

  //triplet such that all 3 elements fit in the span of a half note    
    HALF_TRIP
};*/

struct MeasureNote {
  
  template<typename T>
  MeasureNote( T const & t, float const start, float const l ) :
    note( t ),
    starting_point( start ),
    length( l )
  {}

  std::map< std::string, std::string > tags;
  Note note;
  //Duration duration;
  
  //both measured as fraction of a measure
  float starting_point;
  float length;

  bool
  operator< ( MeasureNote const & other ) const {
    return starting_point < other.starting_point;
  }
};

struct Measure {
public:
  Measure( std::set< MeasureNote > && notes ):
    notes_in_order_( notes )
  {}

  static
  void run_unit_tests();

  MeasureNote const &
  operator[]( int const i ) const {
    return *std::next( notes_in_order_.begin(), i );
  }

  auto begin() {
    return notes_in_order_.begin();
  }

  auto begin() const {
    return notes_in_order_.begin();
  }

  auto end() {
    return notes_in_order_.begin();
  }

  auto end() const {
    return notes_in_order_.begin();
  }

private:
  //std::vector< MeasureNote > notes_in_order_;
  std::set< MeasureNote > notes_in_order_; //inefficient but portable

};

struct TimeSignature {
  signed char top    = 4;
  signed char bottom = 4;
};

class Song {
  std::vector< Measure > measures_;
  //std::vector< GlobalAnnotation > global_annotations_;
  TimeSignature time_signature_;
};

void
Measure::run_unit_tests(){
  { //simple construct test
    Measure const m({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.50 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
      });

    GTT_ASSERT_EQUALS( m[0].starting_point, 0.0 );
    GTT_ASSERT_EQUALS( m[1].starting_point, 0.25 );
    GTT_ASSERT_EQUALS( m[2].starting_point, 0.75 );
  }

  { //out of order test
    Measure const m({
	MeasureNote( "E/4",  0.25, 0.50 ),
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
      });

    GTT_ASSERT_EQUALS( m[0].starting_point, 0.0 );
    GTT_ASSERT_EQUALS( m[1].starting_point, 0.25 );
    GTT_ASSERT_EQUALS( m[2].starting_point, 0.75 );
  }

  { //rest test
    Measure const m({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.25 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
      });
    
  }

}

} // rep
} // gtt
