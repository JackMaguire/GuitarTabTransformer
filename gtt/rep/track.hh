#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/guitar.hh"

//#include <string>
#include <vector>
//#include <set>
//#include <map>

namespace gtt {
namespace rep {

class Track {

private:
  Guitar guitar_ = GuitarFactory::standard_guitar();
  std::vector< Measure > measures_;
};

} // rep
} // gtt
