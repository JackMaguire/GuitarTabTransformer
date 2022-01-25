#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/guitar.hh"

//#include <string>
#include <vector>
//#include <set>
//#include <map>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace gtt {
namespace rep {

struct Track {

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
  }

  void
  deserialize( json const & j ) {
    guitar.deserialize( j[ "guitar" ] );

    int const n_measures = j[ "n_measures" ];
    measures.resize( n_measures );
    for( unsigned int i = 0; i < measures.size(); ++i ){
      measures[ i ].deserialize( j[ std::to_string(i) ] );
    }
  }

  Guitar guitar = GuitarFactory::standard_guitar();
  std::vector< Measure > measures;
};

} // rep
} // gtt
