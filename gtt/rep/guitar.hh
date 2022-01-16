#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/gtrstring.hh"

#include <vector>

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

private:
  std::vector< GtrString > strings_; //from high to low
};

class GuitarFactory {
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
}

} // rep
} // gtt
