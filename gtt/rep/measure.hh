#pragma once

#include "gtt/rep/note.hh"
#include <string>
#include <vector>
//#include <set>
#include <map>

namespace gtt {
namespace rep {

/*template< typename T >
constexpr
T div3( T const t ){
  static_assert( t % 3 == 0 );
  return t / 3;
}

struct NoteDuration {
  using DurationInt = signed char;
  // currently measuring everything in 24th notes to accomodate triplets

  constexpr static DurationInt whole_note = 24;
  constexpr static DurationInt half_note = whole_note/2;
  constexpr static DurationInt quarter_note = whole_note/4;
  constexpr static DurationInt eighth_note = whole_note/8;

  //triplet such that all 3 elements fit in the span of a quarter note
  constexpr static DurationInt quarter_triplet = div3(quarter_note);

  //triplet such that all 3 elements fit in the span of a half note
  constexpr static DurationInt half_triplet = div3(half_note);
};*/

enum class Duration : signed char {
  WHOLE,
    HALF,
    QUARTER,
    EIGHTH,

  //triplet such that all 3 elements fit in the span of a quarter note
    QUARTER_TRIP,

  //triplet such that all 3 elements fit in the span of a half note    
    HALF_TRIP
};

struct MeasureNote {
  std::map< std::string, std::string > tags;
  Note note;
  Duration duration;
};

struct TimeSignature {
  signed char top    = 4;
  signed char bottom = 4;
};

struct Measure {
  std::vector< MeasureNote > notes_in_order;
};

/*class IntraMeasureAnnotation{
  virtual ~IntraMeasureAnnotation();

  virtual void draw() = 0;
};*/

class Measure {
public:

private:
  std::map< std::string, Voice > named_voices_;
  //std::vector< IntraMeasureAnnotation > annotations_;
};

/*class GlobalAnnotation{
  virtual ~GlobalAnnotation();

  virtual void draw() = 0;
};*/

class Song {
  std::vector< Measure > measures_;
  //std::vector< GlobalAnnotation > global_annotations_;
  TimeSignature time_signature_;
};

} // rep
} // gtt
