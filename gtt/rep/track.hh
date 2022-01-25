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

class Track {

  void
  serialize( json & j ) const {
    json gj;
    guitar_.serialize( gj );
    j[ "guitar" ] = gj;

    j[ "n_measures" ] = measures_.size();
    for( unsigned int i = 0; i < measures_.size(); ++i ){
      json mj;
      measures_[i].serialize( mj );
      j[ std::to_string(i) ] = mj;
    }
  }

  void
  deserialize( json const & j ) {
    guitar_.deserialize( j[ "guitar" ] );

    int const n_measures = j[ "n_measures" ];
    measures_.resize( n_measures );
    for( unsigned int i = 0; i < measures_.size(); ++i ){
      measures_[ i ].deserialize( j[ std::to_string(i) ] );
    }
  }

private:
  Guitar guitar_ = GuitarFactory::standard_guitar();
  std::vector< Measure > measures_;
};

} // rep
} // gtt
