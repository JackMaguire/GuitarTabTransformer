//g++ run_unit_tests.cc -I . -std=c++2a -o run_unit_tests -Wall -Wextra -Wshadow -pedantic && ./run_unit_tests

#include "gtt/rep/note.hh"
#include "gtt/rep/gtrstring.hh"
#include "gtt/rep/guitar.hh"
#include "gtt/rep/measure.hh"

int main(){
  gtt::rep::Note::run_unit_tests();
  gtt::rep::GtrString::run_unit_tests();
  gtt::rep::Guitar::run_unit_tests();
  gtt::rep::Measure::run_unit_tests();
}
