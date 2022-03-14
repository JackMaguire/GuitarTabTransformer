#pragma once

#include "gtt/rep/note.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/guitar.hh"

#include "example_songs/spirited_away_intro.hh"

#include <string>
#include <vector>

#include <iostream>

namespace gtt {
namespace render {
namespace ascii {

struct MeasureBoxSettings {
  int measure_width = 32;
  rep::TimeSignature time_sig;

  bool x_is_beat( int const x ) const {
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
    int color = 0;
    short int note_index = -1;
    char c;
  };

  MeasureBox(
    rep::Measure * measure, //not an owner
    rep::Guitar const & g,
    MeasureBoxSettings const & settings
  ){
    initialize( measure, g, settings );
  }

  static
  void run_unit_tests();

public: //utilities
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

  void
  add_note( rep::MeasureNote const & mn ){
    measure_->add( mn );
  }

  int
  annotation_index_for_cell( int const x ) const {
    return ann_for_cell_[ x ];
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
  std::vector< int > ann_for_cell_;

  MeasureBoxSettings settings_;
  rep::Measure * measure_; //not an owner
};

void
MeasureBox::initialize(
  rep::Measure * measure, //not an owner
  rep::Guitar const & g,
  MeasureBoxSettings const & settings
) {
  //using CharVal = MeasureBox::CharVal;

  measure_ = measure;
  settings_ = settings;

  // create empty string
  CharVal const empty_note({ 242, 0, '-' });
  std::vector const empty_string( settings_.measure_width, empty_note );
  render_.resize( g.size()+1, empty_string );

  //annotation_line
  CharVal const empty_ann({ 242, 0, ' ' });
  std::vector const empty_ann_str( settings_.measure_width, empty_ann );
  render_[0] = empty_ann_str;

  ann_for_cell_.assign( settings_.measure_width, -1 );
  int ann_index = -1;
  for( auto const & ann : measure->get_annotations() ){
    ++ann_index;

    int const x = int( settings_.measure_width * ann.starting_point ) - ann.text.size() + 1;
    constexpr int y = 0;
    for( uint i = 0; i < ann.text.size(); ++i ){
      render_[y][x+i].c = ann.text[ i ];
      render_[y][x+i].color = ann.color;

      ann_for_cell_[x+i] = ann_index;
    }
  }

  // color the beats
  for( int const x : settings_.beat_positions() ){
    for( uint y = 1; y <= g.size(); ++y ){
      render_[y][x].color = 252;
    }
  }

  // draw notes
  char note_index = 0;
  for( rep::MeasureNote const & mnote : *measure ){
    int const y = mnote.string_assignment+1;
    int const x = int( settings_.measure_width * mnote.starting_point );
    int const fret = g[ mnote.string_assignment ].get_fret( mnote.note );
    std::string const fret_str = std::to_string( fret );

    if( fret_str.size() == 1 ){
      render_[y][x].c = fret_str[ 0 ];
      render_[y][x].color = 0;
      render_[y][x].note_index = note_index;
    } else {
      GTT_ASSERT_EQUALS( fret_str.size(), 2 );
      if( x == 0 ){
	render_[y][x].c = fret_str[ 0 ];
	render_[y][x+1].c = fret_str[ 1 ];
	render_[y][x].color = 0;
	render_[y][x+1].color = 0;
	render_[y][x].note_index = note_index;
	render_[y][x+1].note_index = note_index;
      } else {
	render_[y][x-1].c = fret_str[ 0 ];
	render_[y][x].c = fret_str[ 1 ];
	render_[y][x-1].color = 0;
	render_[y][x].color = 0;
	render_[y][x-1].note_index = note_index;
	render_[y][x].note_index = note_index;
      }
    }

    ++note_index;
  }

}

void
MeasureBox::run_unit_tests(){
  rep::Track t = gtt::spirited_away_intro();

  GTT_ASSERT_EQUALS( t.measures.size(), 4 );

  /*
    E/4 ----------------------------0--- -------------------------------- --------------------------------
    B/3 ----0---0---0---0-------0------- 0------------------------------- ----------------------------3---
    G/3 --------------------2----------- ----2-2------------------------- ----2---2---2---2---0---2-------
    D/3 -------------------------------- -------------------------------- --------------------------------
    A/2 -------------------------------- -------------------------------- --------------------------------
    E/2 -------------------------------- -------------------------------- --------------------------------
   */

  MeasureBoxSettings settings;
  settings.time_sig = t.time_signature;

  { // measure 0
    MeasureBox const mbox(
      & t.measures[0],
      t.guitar,
      settings
    );

    GTT_ASSERT_EQUALS( t.measures[0].size(), 7 );

    auto && tester = [&]( int const str, std::string const & line ){ //line 0
      GTT_ASSERT_EQUALS( int(line.size()), mbox.width() );
      for( int i = 0; i < mbox.width(); ++i ){
	//std::cout << line[i] << " " << mbox.at( i, 0 ).c << std::endl;
	GTT_ASSERT_EQUALS( line[i], mbox.at( i, str ).c );
	if( mbox.at( i, str ).c != '-' ){
	  GTT_ASSERT_EQUALS( mbox.at( i, str ).color, 0 );
	} else {
	  GTT_ASSERT_NOT_EQUALS( mbox.at( i, str ).color, 0 );
	}
      }
    };

    // Test beat
    GTT_ASSERT_EQUALS( mbox.at( 0, 0 ).color, 252 );

    // Test non-beat
    GTT_ASSERT_EQUALS( mbox.at( 1, 0 ).color, 242 );

    // Test note
    GTT_ASSERT_EQUALS( mbox.at( 4, 1 ).color, 0 );

    tester( 0, "----------------------------0---" );
    tester( 1, "----0---0---0---0-------0-------" );
    tester( 2, "--------------------2-----------" );
    tester( 3, "--------------------------------" );
    tester( 4, "--------------------------------" );
    tester( 5, "--------------------------------" );
  }
}

} // ascii
} // render
} // gtt
