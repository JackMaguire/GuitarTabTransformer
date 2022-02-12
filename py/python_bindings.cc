//g++ py/python_bindings.cc -o gtt$(python3-config --extension-suffix) -O3 -Wall -Wextra -I . -isystem extern/json/single_include/ -std=c++2a -fPIC $(python3 -m pybind11 --includes) -shared

#include <iostream>

#include "gtt/rep/note.hh"
#include "gtt/rep/gtrstring.hh"
#include "gtt/rep/guitar.hh"
#include "gtt/rep/measure.hh"
#include "gtt/rep/track.hh"
#include "gtt/asserts.hh"

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace gtt;
using namespace gtt::rep;

PYBIND11_MODULE(gtt, m) {
    m.doc() = "Guitar Tab Transformer";
    py::module shortcuts = m.def_submodule( "shortcuts" );

    py::enum_< Note::Letter >( m, "NoteLetter" )
      .value( "C", Note::Letter::C )
      .value( "Db", Note::Letter::Db )
      .value( "D", Note::Letter::D )
      .value( "Eb", Note::Letter::Eb )
      .value( "E", Note::Letter::E )
      .value( "F", Note::Letter::F )
      .value( "Gb", Note::Letter::Gb )
      .value( "G", Note::Letter::G )
      .value( "Ab", Note::Letter::Ab )
      .value( "A", Note::Letter::A )
      .value( "Bb", Note::Letter::Bb )
      .value( "B", Note::Letter::B )
      .value( "COUNT", Note::Letter::COUNT )
      ;

    py::class_< Note > note( m, "Note" );
    note.def( py::init<>() );
    note.def( py::init< Note::Letter, Note::Octave >() );
    note.def( py::init< std::string >() );
    note.def( py::init< int >() );
    note.def( py::init< Note const & >() );

    note.def( "run_unit_tests", &Note::run_unit_tests );

    note.def( "letter2string", &Note::letter2string );
    note.def( "string2letter", &Note::string2letter );
    note.def( "char2letter", &Note::char2letter );
    shortcuts.def( "s2l", &Note::string2letter );
    shortcuts.def( "l2s", &Note::letter2string );

    note.attr( "HalfStep" ) = py::int_( Note::HalfStep );
    note.attr( "WholeStep" ) = py::int_( Note::WholeStep );
    note.attr( "OctaveStep" ) = py::int_( Note::OctaveStep );
    shortcuts.attr( "half" ) = py::int_( Note::HalfStep );
    shortcuts.attr( "whole" ) = py::int_( Note::WholeStep );
    shortcuts.attr( "oct" ) = py::int_( Note::OctaveStep );

    note.def( "init_from_string", &Note::init_from_string );
    note.def( "as_string", &Note::as_string );
    note.def( "init_from_int", &Note::init_from_int );
    note.def( "as_int", &Note::as_int );
    note.def( "__repr__", &Note::as_string );
    
    note.def( py::self + int() );
    note.def( py::self += int() );
    note.def( py::self - int() );
    note.def( py::self -= int() );

    note.def( py::self <  py::self );
    note.def( py::self <= py::self );
    note.def( py::self == py::self );
    note.def( py::self != py::self );
    note.def( py::self >= py::self );
    note.def( py::self >  py::self );
}
