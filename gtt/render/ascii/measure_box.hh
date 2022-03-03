#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/guitar.hh"


namespace gtt {
namespace render {
namespace ascii {

struct MeasureBoxSettings {
  int measure_width = 24;
  rep::TimeSignature time_sig;
};

class MeasureBox {
public:
  
  struct CharVal {
    char c;
    int color;
  };

  MeasureBox(
    rep::Measure * measure, //not an owner
    rep::Guitar const & g,
    MeasureBoxSettings const & settings
  ){
    initialize( measure, g, settings );
  }

protected:
  void
  initialize(
    rep::Measure * measure, //not an owner
    rep::Guitar const & g,
    MeasureBoxSettings const & settings
  );

private:
  std::vector< std::vector< CharVal > > render_;

  MeasureBoxSettings settings_;
  rep::Measure * measure_; //not an owner
};

void
MeasureBox::initialize(
  rep::Measure * measure, //not an owner
  rep::Guitar const & g,
  MeasureBoxSettings const & settings
) {
  using MeasureBox::CharVal;

  measure_ = measure;
  settings_ = settings;

  // create empty string
  constexpr CharVal empty_note({ '-', 242 });
  constexpr std::vector empty_string( settings_.measure_width, empty_note );
  render_.resize( g.size(), empty_string );
}


} // ascii
} // render
} // gtt
