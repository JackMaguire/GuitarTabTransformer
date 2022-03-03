#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/guitar.hh"

#include <string>
#include <vector>

namespace gtt {
namespace render {
namespace ascii {

struct MeasureBoxSettings {
  int measure_width = 24;
  rep::TimeSignature time_sig;

  bool x_is_beat( int const x ) const {
    //if( x < 0 ) return false;
    //if( x >= measure_width ) return false;
    int const beat_size = measure_width / time_sig.beats_per_measure();
    return x % beat_size == 0;
  }

  std::vector< int >
  beat_positions() const {
    std::vector< int > beats;
    beats.reserve( time_sig.beats_per_measure() );
    int const beat_size = measure_width / time_sig.beats_per_measure();
    for( int i = 0; i < time_sig.beats_per_measure(); ++i ){
      beats.push_back( i * beat_size );
    }
    return beats;
  }

};

class MeasureBox {
public:
  
  struct CharVal {
    int color;
    char c;
  };

  MeasureBox(
    rep::Measure * measure, //not an owner
    rep::Guitar const & g,
    MeasureBoxSettings const & settings
  ){
    initialize( measure, g, settings );
  }

  int width() const {
    if( render_.empty() ) return 0;
    return render_[0].size();
  }

  int height() const {
    return render_.size();
  }

  CharVal
  at( int const x, int const y ) const {
    GTT_ASSERT( x >= 0 and x < width() );
    GTT_ASSERT( y >= 0 and y < height() );
    return render_[ y ][ x ];
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

  // color the beats
  for( int const x : settings_->beat_positions() ){
    for( int y = 0; y < g.size(); ++y ){
      render_[y][x].color = 252;
    }
  }

  // draw notes
  for( MeasureNote const & mnote : *measure ){
    int const y = mnote.string_assignment;
    int const x = int( settings_->measure_width * mnote.starting_point );
    int const fret = g.get_string( y ).get_fret( mnote.note );
    std::string const fret_str = std::to_string( fret );
    if( fret_str.size() == 1 ){
      render_[y][x].c = fret_str[ 0 ];
    } else {
      GTT_ASSERT_EQUALS( fret_str.size(), 2 );
      if( x == 0 ){
	render_[y][x].c = fret_str[ 0 ];
	render_[y][x+1].c = fret_str[ 1 ];
      } else {
	render_[y][x-1].c = fret_str[ 0 ];
	render_[y][x].c = fret_str[ 1 ];
      }
    }
  }
}


} // ascii
} // render
} // gtt
