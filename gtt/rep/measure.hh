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

constexpr float EIGHTH = 1.0 / 8.0;

struct MeasureNote {

  template<typename T>
  MeasureNote(
    T const & t,
    float const start,
    float const l,
    bool const rest = false
  );

  template<typename T>
  MeasureNote(
    T const & t,
    Guitar const & guitar,
    float const start,
    float const l,
    bool const rest = false
  );

  MeasureNote( json const & j ){
    deserialize( j );
  }

  bool
  operator< ( MeasureNote const & other ) const;

  float
  ending_point() const {
    return starting_point + length;
  }

public: //serialization
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

public:
  /*
    DATA
   */
  Note note;
  bool is_rest = false;

  //both measured as fraction of a measure
  float starting_point;
  float length;

  //which string do we play this on?
  signed char string_assignment = 0;
};

struct MeasureAnnotation {

  MeasureAnnotation() = default;

  MeasureAnnotation( json const & j ){
    deserialize( j );
  }


  bool
  operator< ( MeasureAnnotation const & other ) const {
    if( starting_point != other.starting_point )
      return starting_point < other.starting_point;

    return text < other.text;
  }

  void
  serialize( json & j ) const {
    j[ "text" ] = text;
    j[ "starting_point" ] = starting_point;
    j[ "color" ] = color;
  }

  void
  deserialize( json const & j ) {
    text = j[ "text" ];
    starting_point = j[ "starting_point" ];
    color = j[ "color" ];
  }

  std::string text;
  float starting_point;
  int color = 3;
};

class Measure {
public:
  Measure() = default;

  Measure( std::set< MeasureNote > && notes ):
    notes_in_order_( notes )
  {}

  static
  void run_unit_tests();

  std::set< MeasureNote >
  compute_rests() const;

  void
  set( std::set< MeasureNote > && notes ){
    notes_in_order_ = notes;
  }

public: //note utilities
  MeasureNote const &
  operator[]( int const i ) const {
    return *std::next( notes_in_order_.begin(), i );
  }

  auto size() const {
    return notes_in_order_.size();
  }

  auto begin() {
    return notes_in_order_.begin();
  }

  auto begin() const {
    return notes_in_order_.begin();
  }

  auto end() {
    return notes_in_order_.end();
  }

  auto end() const {
    return notes_in_order_.end();
  }

  void
  add( MeasureNote const & mn ) {
    notes_in_order_.insert( mn );
  }

  void
  remove( int const index ){
    notes_in_order_.erase( std::next( notes_in_order_.begin(), index ) );
  }

public: //annotation utilities
  void
  add_annotation( MeasureAnnotation const & ann ){
    annotations_.push_back( ann );
  }

  auto
  n_annotations() const {
    return annotations_.size();
  }

  void
  remove_annotation( int const index ){
    annotations_.erase( std::next( annotations_.begin(), index ) );
  }

  MeasureAnnotation &
  get_annotation( int const index ) {
    return annotations_[ index ];
  }

  auto const &
  get_annotations() const {
    return annotations_;
  }

  void
  toggle_annotation_color( int const index ){
    annotations_[ index ].color = (annotations_[ index ].color+1) % 10;
  }

public: //quality of life in python
  void
  change_string_assignment( int const index, int const assignment ){
    MeasureNote copy = (*this)[index];
    remove( index );
    copy.string_assignment = assignment;
    add( copy );
  }

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

    for( MeasureAnnotation const & ann : annotations_ ){
      json j2;
      ann.serialize( j2 );
      j[ std::to_string(count++) ] = j2;
    }
    j[ "ann_count" ] = count;

    j[ "total_count" ] = count;
  }

  void
  deserialize( json const & j ) {
    notes_in_order_.clear();

    int const count = j[ "count" ];
    for( int i = 0; i < count; ++i ){
      json const j2 = j[ std::to_string(i) ];
      notes_in_order_.emplace( j[ std::to_string(i) ] );
    }

    if( j.contains( "ann_count" ) ){
      int const ann_count = j[ "ann_count" ];
      for( int i = count; i < ann_count; ++i ){
	json const j2 = j[ std::to_string(i) ];
	annotations_.emplace_back( j[ std::to_string(i) ] );
      }
    }
  }


private:
  std::set< MeasureNote > notes_in_order_; //inefficient but easy
  std::vector< MeasureAnnotation > annotations_;
};

template< typename T >
MeasureNote::MeasureNote(
  T const & t,
  float const start,
  float const l,
  bool const rest
) :
  note( t ),
  is_rest( rest ),
  starting_point( start ),
  length( l )
{}

template<typename T>
MeasureNote::MeasureNote(
  T const & t,
  Guitar const & guitar,
  float const start,
  float const l,
  bool const rest
) :
  note( t ),
  is_rest( rest ),
  starting_point( start ),
  length( l )
{
  string_assignment = guitar.highest_string_for_note( note );
}

bool
MeasureNote::operator< ( MeasureNote const & other ) const {
  if( starting_point != other.starting_point )
    return starting_point < other.starting_point;

  if( length != other.length )
    return length < other.length;

  if( note != other.note )
    return note < other.note;

  return string_assignment < other.string_assignment;
}


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

void
Measure::run_unit_tests(){
  { //simple construct and serialize test
    Measure const m({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.50 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
      });

    GTT_ASSERT_EQUALS( m[0].starting_point, 0.0 );
    GTT_ASSERT_EQUALS( m[1].starting_point, 0.25 );
    GTT_ASSERT_EQUALS( m[2].starting_point, 0.75 );

    json j;
    m.serialize( j );

    Measure m2({
	MeasureNote( "E/2",  0.25, 0.40 ),
	MeasureNote( "Ab/1", 0.00, 0.45 ),
      });
    m2.deserialize( j );

    GTT_ASSERT_EQUALS( m2.size(), 3 );
    GTT_ASSERT_EQUALS( m2[0].starting_point, 0.0 );
    GTT_ASSERT_EQUALS( m2[1].starting_point, 0.25 );
    GTT_ASSERT_EQUALS( m2[2].starting_point, 0.75 );
    GTT_ASSERT_EQUALS( m2[0].length, 0.25 );
    GTT_ASSERT_EQUALS( m2[1].length, 0.50 );
    GTT_ASSERT_EQUALS( m2[2].length, 0.25 );
    GTT_ASSERT_EQUALS( m2[0].note.as_int(), "Ab/5"_note.as_int() );
    GTT_ASSERT_EQUALS( m2[1].note.as_int(),  "E/4"_note.as_int() );
    GTT_ASSERT_EQUALS( m2[2].note.as_int(), "Gb/6"_note.as_int() );

    auto const & rests = m2.compute_rests();
    GTT_ASSERT_EQUALS( rests.size(), 0 );
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
