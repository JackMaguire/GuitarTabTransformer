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

    


    py::class_< GtrString > gtr_str( m, "GtrString" );
    gtr_str.def( py::init<>() );
    gtr_str.def( py::init< int >() );
    gtr_str.def( py::init< Note >() );
    gtr_str.def( py::init< int, int >() );
    gtr_str.def( py::init< Note, int >() );
    
    gtr_str.def( "run_unit_tests", &GtrString::run_unit_tests );
    gtr_str.def( "get_fret", &GtrString::get_fret );
    gtr_str.def( "can_represent", &GtrString::can_represent );
    gtr_str.def( "open_string_note", &GtrString::open_string_note );
    gtr_str.def( "set_open_string_note", &GtrString::set_open_string_note );
    gtr_str.def( "max_fret", &GtrString::max_fret );
    gtr_str.def( "set_max_fret", &GtrString::set_max_fret );
    gtr_str.def( "serialize", &GtrString::serialize );
    gtr_str.def( "deserialize", &GtrString::deserialize );




    py::class_< Guitar > gtr( m, "Guitar" );
    gtr.def( py::init<>() );
    gtr.def( py::init< Guitar const & >() );
    gtr.def( py::init< std::vector< std::string > const & >() );
    
    gtr.def( "run_unit_tests", &Guitar::run_unit_tests );
    gtr.def( "highest_string_for_note", &Guitar::highest_string_for_note );
    gtr.def( "size", &Guitar::size );
    gtr.def( "__len__", &Guitar::size );
    gtr.def( "__getitem__", [](Guitar const & g, size_t index) {return &g[index];} );
    gtr.def( "__setitem__", [](Guitar& g, size_t index) { return &g[index];} );
    gtr.def( "serialize", &Guitar::serialize );
    gtr.def( "deserialize", &Guitar::deserialize );




    py::class_< GuitarFactory > gtr_factory( m, "GuitarFactory" );
    gtr_factory.def( "standard_guitar", &GuitarFactory::standard_guitar );
    gtr_factory.def( "dropD_guitar", &GuitarFactory::dropD_guitar );
    gtr_factory.def( "standard_seven_string_guitar",
      &GuitarFactory::standard_seven_string_guitar );

    gtr_factory.def( "standard_bass_guitar", &GuitarFactory::standard_bass_guitar );
    gtr_factory.def( "dropD_bass_guitar", &GuitarFactory::dropD_bass_guitar );
    gtr_factory.def( "standard_five_string_bass_guitar",
      &GuitarFactory::standard_five_string_bass_guitar );



    py::class_< MeasureNote > mnote( m, "MeasureNote" );

    mnote.def( py::init< Note const &, float, float >() );
    mnote.def( py::init< Note const &, float, float, bool >() );
    mnote.def( py::init< int, float, float >() );
    mnote.def( py::init< int, float, float, bool >() );

    mnote.def( py::init< Note const &, Guitar const &, float, float >() );
    mnote.def( py::init< Note const &, Guitar const &, float, float, bool >() );
    mnote.def( py::init< int, Guitar const &, float, float >() );
    mnote.def( py::init< int, Guitar const &, float, float, bool >() );
    mnote.attr( "EIGHTH" ) = py::float_( EIGHTH );

    mnote.def( py::self <  py::self );
    mnote.def( "ending_point", &MeasureNote::ending_point );
    mnote.def( "serialize", &MeasureNote::serialize );
    mnote.def( "deserialize", &MeasureNote::deserialize );

    mnote.def_readwrite( "note", &MeasureNote::note );
    mnote.def_readwrite( "is_rest", &MeasureNote::is_rest );
    mnote.def_readwrite( "starting_point", &MeasureNote::starting_point );
    mnote.def_readwrite( "length", &MeasureNote::length );
    mnote.def_readwrite( "string_assignment", &MeasureNote::string_assignment );




    py::class_< Measure > measure( m, "Measure" );
    measure.def( py::init<>() );
    measure.def( py::init< std::set< MeasureNote > && >() );
    measure.def( "run_unit_tests", &Measure::run_unit_tests );

    measure.def( "size", &Measure::size );
    measure.def( "__len__", &Measure::size );
    measure.def("__iter__",
      []( Measure & meas ) {
	return py::make_iterator( meas.begin(), meas.end() );
      }, py::keep_alive<0, 1>() );
    measure.def( "__getitem__",
      []( Measure const & g, size_t index ) {return &g[index];}
    );

    measure.def( "compute_rests", &Measure::compute_rests );
    measure.def( "set", &Measure::set );
    measure.def( "add", &Measure::add );

    measure.def( "serialize", &Measure::serialize );
    measure.def( "deserialize", &Measure::deserialize );


    py::class_< Track > track( m, "Track" );
    track.def( py::init<>() );

    track.def_readwrite( "guitar", &Track::guitar );
    track.def_readwrite( "measures", &Track::measures );

    track.def( "serialize", &Track::serialize );
    track.def( "deserialize", &Track::deserialize );

}