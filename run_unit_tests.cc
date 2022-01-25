//g++ run_unit_tests.cc -I . -I extern/json/single_include/ -std=c++2a -o run_unit_tests -Wall -Wextra -Wshadow -pedantic && ./run_unit_tests

#include "gtt/rep/note.hh"
#include "gtt/rep/gtrstring.hh"
#include "gtt/rep/guitar.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/track.hh"


#include "example_songs/spirited_away_intro.hh"

int main(){
  gtt::rep::Note::run_unit_tests();
  gtt::rep::GtrString::run_unit_tests();
  gtt::rep::Guitar::run_unit_tests();
  gtt::rep::Measure::run_unit_tests();


  auto const t = gtt::spirited_away_intro();
  json j;
  t.serialize( j );
  std::cout << j.dump() << std::endl;
}
