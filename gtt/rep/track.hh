#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/guitar.hh"

//#include <string>
#include <vector>
#include <fstream>
//#include <set>
//#include <map>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace gtt {
namespace rep {

struct TimeSignature {
  unsigned char top    = 4;
  unsigned char bottom = 4;

  unsigned char
  beats_per_measure() const {
    return top;
  }
}

struct Track {

  Track() = default;
  Track( Track const & ) = default;
  Track( std::string const & filename ){
    load_from_file( filename );
  }

  void
  save_to_file( std::string const & filename ) const {
    json j;
    serialize( j );

    std::ofstream file( filename );
    file << j;
  }

  void
  serialize( json & j ) const {
    json gj;
    guitar.serialize( gj );
    j[ "guitar" ] = gj;

    j[ "n_measures" ] = measures.size();
    for( unsigned int i = 0; i < measures.size(); ++i ){
      json mj;
      measures[i].serialize( mj );
      j[ std::to_string(i) ] = mj;
    }

    j[ "time_signature_top" ] = time_signature.top;
    j[ "time_signature_bottom" ] = time_signature.bottom;
  }

  void
  load_from_file( std::string const & filename ) {
    std::ifstream ifs( filename );
    json j = json::parse(ifs);
    deserialize( j );
  }

  void
  deserialize( json const & j ) {
    guitar.deserialize( j[ "guitar" ] );

    int const n_measures = j[ "n_measures" ];
    measures.resize( n_measures );
    for( unsigned int i = 0; i < measures.size(); ++i ){
      measures[ i ].deserialize( j[ std::to_string(i) ] );
    }

    if( j.contains("time_signature_top") ){
      time_signature.top = j[ "time_signature_top" ];
    }

    if( j.contains("time_signature_bottom") ){
      time_signature.bottom = j[ "time_signature_bottom" ];
    }
  }

  Guitar guitar = GuitarFactory::standard_guitar();
  std::vector< Measure > measures;
  TimeSignature time_signature;
};

} // rep
} // gtt
