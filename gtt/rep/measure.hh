#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/guitar.hh"

#include <string>
#include <vector>
#include <set>
#include <map>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace gtt {
namespace rep {

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
  MeasureNote(
    T const & t,
    float const start,
    float const l,
    bool const rest = false
  ) :
    note( t ),
    is_rest( rest ),
    starting_point( start ),
    length( l )
  {}

  template<typename T>
  MeasureNote(
    T const & t,
    Guitar const & guitar,
    float const start,
    float const l,
    bool const rest = false
  ) :
    note( t ),
    is_rest( rest ),
    starting_point( start ),
    length( l )
  {
    string_assignment = guitar.highest_string_for_note( note );
  }

  MeasureNote( json const & j ){
    deserialize( j );
  }

  bool
  operator< ( MeasureNote const & other ) const {
    return starting_point < other.starting_point;
  }

  float
  ending_point() const {
    return starting_point + length;
  }

  void
  serialize( json & j ) const {
    j[ "note" ] = note.as_string();
    j[ "is_rest" ] = is_rest;
    j[ "starting_point" ] = starting_point;
    j[ "length" ] = length;
    j[ "string_assignment" ] = int(string_assignment);
  }

  void
  deserialize( json const & j ) {
    note.init_from_string( j[ "note" ] );
    is_rest = j[ "is_rest" ];
    starting_point = j[ "starting_point" ];
    length = j[ "length" ];
    string_assignment = j[ "string_assignment" ];
  }

  /*
    DATA
   */

  //std::map< std::string, std::string > tags;
  Note note;
  bool is_rest = false;
  
  //both measured as fraction of a measure
  float starting_point;
  float length;

  //which string do we play this on?
  signed char string_assignment = 0;
};

class Measure {
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

  std::set< MeasureNote >
  compute_rests() const;

public: //serialization
  void
  serialize( json & j ) const {
    int count = 0;
    for( MeasureNote const & mn : notes_in_order_ ){
      json j2;
      mn.serialize( j2 );
      j[ std::to_string(count++) ] = j2;
    }
    j[ "count" ] = count;
  }

  void
  deserialize( json const & j ) {

    notes_in_order_.clear();

    int const count = j[ "count" ];
    for( int i = 0; i < count; ++i ){
      json const j2 = j[ std::to_string(i) ];
      notes_in_order_.emplace( j[ std::to_string(i) ] );
    }
  }


private:
  //std::vector< MeasureNote > notes_in_order_;
  std::set< MeasureNote > notes_in_order_; //inefficient but portable
};

std::set< MeasureNote >
Measure::compute_rests() const {
  std::set< MeasureNote > rests;

  //full-measure rest
  if( notes_in_order_.empty() ){
    rests.emplace( 0, 0.0, 1.0, true );
    return rests;
  }

  {//front
    MeasureNote const & first = *notes_in_order_.begin();
    if( first.starting_point > 0.0 ){
      rests.emplace( 0, 0.0, first.starting_point, true );
    }
  }

  {//back
    MeasureNote const & back = *std::prev( notes_in_order_.end() );
    float const back_end = back.ending_point();
    float const diff = 1.0 - back_end;
    if( diff > 0.00001 ){
      rests.emplace( 0, back_end, diff, true );
    }
  }

  for( auto iter = notes_in_order_.begin();
       iter != notes_in_order_.end() and
	 std::next(iter) != notes_in_order_.end();) {
    MeasureNote const & a = *iter;
    MeasureNote const & b = *(++iter);
    float const a_end = a.ending_point();
    float const diff = b.starting_point - a_end;
    if( diff > 0.00001 ){
      rests.emplace( 0, a_end, diff, true );
    }
  }

  return rests;
}

/*struct TimeSignature {
  signed char top    = 4;
  signed char bottom = 4;
};

class Song {
  std::vector< Measure > measures_;
  //std::vector< GlobalAnnotation > global_annotations_;
  TimeSignature time_signature_;
};*/

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

  { //single rest test
    Measure const m({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.25 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
      });

    auto const & rests = m.compute_rests();
    GTT_ASSERT_EQUALS( rests.size(), 1 );

    auto const & rest = *rests.begin();
    GTT_ASSERT_EQUALS( rest.starting_point, 0.5 );
    GTT_ASSERT_EQUALS( rest.length, 0.25 );
    GTT_ASSERT_EQUALS( rest.ending_point(), 0.75 );
  }

  { //multiple rest test
    Measure const m({
	MeasureNote( "Ab/5", 0.25, 0.125 ),
	MeasureNote( "Gb/6", 0.75, 0.125 )
      });

    auto const & rests = m.compute_rests();
    GTT_ASSERT_EQUALS( rests.size(), 3 );

    auto iter = rests.begin();
    {
      auto const & rest = *iter;
      GTT_ASSERT_EQUALS( rest.starting_point, 0.0 );
      GTT_ASSERT_EQUALS( rest.length, 0.25 );
      GTT_ASSERT_EQUALS( rest.ending_point(), 0.25 );
    }

    ++iter;
    {
      auto const & rest = *iter;
      GTT_ASSERT_EQUALS( rest.starting_point, 0.375 );
      GTT_ASSERT_EQUALS( rest.length, 0.375 );
      GTT_ASSERT_EQUALS( rest.ending_point(), 0.75 );
    }

    ++iter;
    {
      auto const & rest = *iter;
      GTT_ASSERT_EQUALS( rest.starting_point, 0.875 );
      GTT_ASSERT_EQUALS( rest.length, 0.125 );
      GTT_ASSERT_EQUALS( rest.ending_point(), 1.0 );
    }
  }

  { //overlapping notes
    Measure const m({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.25 ),
	MeasureNote( "E/7",  0.25, 0.125 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
      });

    auto const & rests = m.compute_rests();
    GTT_ASSERT_EQUALS( rests.size(), 1 );

    auto const & rest = *rests.begin();
    GTT_ASSERT_EQUALS( rest.starting_point, 0.5 );
    GTT_ASSERT_EQUALS( rest.length, 0.25 );
    GTT_ASSERT_EQUALS( rest.ending_point(), 0.75 );
  }

  { //auto-assign strings
    Guitar const g = GuitarFactory::standard_guitar();
    Measure const m({
	MeasureNote( "G/3"_note +3, g, 0.00, 0.25 ),
	MeasureNote( "A/2",  g, 0.25, 0.25 ),
	MeasureNote( "D/3"_note -1, g, 0.75, 0.25 )
      });

    auto iter = m.begin();
    GTT_ASSERT_EQUALS( iter->string_assignment, 2 );

    ++iter;
    GTT_ASSERT_EQUALS( iter->string_assignment, 4 );

    ++iter;
    GTT_ASSERT_EQUALS( iter->string_assignment, 4 );
    
  }
}

} // rep
} // gtt
