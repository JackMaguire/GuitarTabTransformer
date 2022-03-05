#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/gtrstring.hh"

#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace gtt {
namespace rep {

class Guitar {
public:
  Guitar() = default;
  Guitar( Guitar const & ) = default;

  Guitar( std::vector< std::string > const & strings ){
    strings_.reserve( strings.size() );
    for( std::string const & s : strings ){
      strings_.emplace_back( s );
    }
  }

  template< int I >
  Guitar( std::array< std::string, I > const & strings ){
    strings_.reserve( strings.size() );
    for( std::string const & s : strings ){
      strings_.emplace_back( s );
    }
  }

  static
  void run_unit_tests();

  signed char
  highest_string_for_note( Note const & note ) const {
    for( unsigned int i = 0; i < strings_.size(); ++i ){
      if( strings_[i].can_represent( note ) ) return i;
    }
    return strings_.size()-1;//last string
  }

public:
  GtrString &
  get_string( int const i ) {
    return strings_[ i ];
  }

  GtrString *
  get_string_ptr( int const i ) {
    return &strings_[ i ];
  }

  GtrString &
  operator[]( int const i ) {
    return strings_[ i ];
  }

  GtrString const &
  operator[]( int const i ) const {
    return strings_[ i ];
  }

  auto size() const {
    return strings_.size();
  }

public: //serialization
  void
  serialize( json & j ) const {
    j[ "nstrings" ] = strings_.size();
    for( unsigned int i = 0; i < strings_.size(); ++i ){
      json j2;
      strings_[ i ].serialize( j2 );
      j[ std::to_string(i) ] = j2;
    }
  }

  void
  deserialize( json const & j ) {
    strings_.resize( j[ "nstrings" ] );
    for( unsigned int i = 0; i < strings_.size(); ++i ){
      json const j2 = j[ std::to_string(i) ];
      strings_[ i ].deserialize( j2 );
    }
  }

private:
  std::vector< GtrString > strings_; //from high to low
};

class GuitarFactory {
public:
  /*
    GUITARS
   */
  static
  Guitar
  standard_guitar(){
    return Guitar({
      "E/4",
      "B/3",
      "G/3",
      "D/3",
      "A/2",
      "E/2"
    });
  }

  static
  Guitar
  dropD_guitar(){
    return Guitar({
      "E/4",
      "B/3",
      "G/3",
      "D/3",
      "A/2",
      "D/2"
    });
  }

  static
  Guitar
  standard_seven_string_guitar(){
    return Guitar({
      "E/4",
      "B/3",
      "G/3",
      "D/3",
      "A/2",
      "E/2",
      "B/1"
    });
  }

  /*
    BASS
   */
  static
  Guitar
  standard_bass_guitar(){
    return Guitar({
      "G/2",
      "D/2",
      "A/1",
      "E/1"
    });
  }

  static
  Guitar
  dropD_bass_guitar(){
    return Guitar({
      "G/2",
      "D/2",
      "A/1",
      "D/1"
    });
  }

  static
  Guitar
  standard_five_string_bass_guitar(){
    return Guitar({
      "G/2",
      "D/2",
      "A/1",
      "E/1",
      "B/0"
    });
  }
};

void
Guitar::run_unit_tests(){
  {//test serialization
    json j;
    Guitar g1 = GuitarFactory::dropD_bass_guitar();
    g1.serialize( j );

    //std::cout << j.dump() << std::endl;

    Guitar g2 = GuitarFactory::standard_seven_string_guitar();
    g2.deserialize( j );

    GTT_ASSERT_EQUALS( g2.strings_.size(), 4 );
    GTT_ASSERT_EQUALS( g2.strings_[0].open_string_note().as_int(),
      "G/2"_note.as_int() );
    GTT_ASSERT_EQUALS( g2.strings_[1].open_string_note().as_int(),
      "D/2"_note.as_int() );
    GTT_ASSERT_EQUALS( g2.strings_[2].open_string_note().as_int(),
      "A/1"_note.as_int() );
    GTT_ASSERT_EQUALS( g2.strings_[3].open_string_note().as_int(),
      "D/1"_note.as_int() );

  }

}

} // rep
} // gtt
