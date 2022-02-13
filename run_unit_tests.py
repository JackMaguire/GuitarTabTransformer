from gtt import *
import gtt.shortcuts as X

def run_X_unit_tests():
    # Block 1
    n = Note( "G/4" )

    print( "Passed X Tests" )


def run_Note_unit_tests():
    # Block 1
    n = Note( "G/4" )
    assert( n.letter() == NoteLetter.G )
    assert( n.octave() == 4 )

    n2 = Note( n.as_int() )
    assert( n == n2 )

    g3 = n - X.oct
    assert( g3.letter() == NoteLetter.G )
    assert( g3.octave() == 3 )


    # Block 2
    n = Note( "Ab/0" )
    assert( n.letter() == NoteLetter.Ab )
    assert( n.octave() == 0 )

    n += 25
    assert( n.letter() == NoteLetter.A )
    assert( n.octave() == 2 )


    # Block 3
    assert( Note( "A/1" ) != Note( "B/1" ) );
    assert( Note( "A/1" ) <  Note( "B/1" ) );
    assert( Note( "A/1" ) <= Note( "A/1" ) );
    assert( Note( "A/1" ) == Note( "A/1" ) );
    assert( Note( "A/1" ) >= Note( "A/1" ) );
    assert( Note( "G/5" ) >  Note( "A/4" ) );
    assert( Note( "G/5" ) != Note( "A/4" ) );

    print( "Passed Note Tests" )

def run_GtrString_unit_tests():
    s = GtrString( "Ab/5", 24 )
    assert( s.get_fret( s.open_string_note() ) == 0 )
    for i in range( -20, 21 ):
        assert( s.get_fret( s.open_string_note()+i ) == i )

    assert( not s.can_represent( Note( "Ab/4" ) ) );
    assert( not s.can_represent( Note( "Ab/5" )-1 ) );
    assert( s.can_represent( Note( "Ab/5" ) ) );
    assert( s.can_represent( Note( "Ab/6" ) ) );
    assert( s.can_represent( Note( "Ab/7" )-1 ) );
    assert( s.can_represent( Note( "Ab/7" ) ) );
    assert( not s.can_represent( Note( "Ab/7" )+1 ) );
    assert( not s.can_represent( Note( "Ab/8" ) ) );

    s.set_max_fret( 25 );
    assert( s.can_represent( Note( "Ab/7" )+1 ) );
    assert( not s.can_represent( Note( "Ab/8" ) ) );


    print( "Passed GtrString Tests" )

def run_Guitar_unit_tests():
    g = Guitar( GuitarFactory.dropD_bass_guitar() )

    assert( g.size() == 4 );
    assert( len(g) == 4 );

    assert( g[0].open_string_note().as_int() ==
            Note("G/2").as_int() );
    assert( g[1].open_string_note().as_int() ==
            Note("D/2").as_int() );
    assert( g[2].open_string_note().as_int() ==
            Note("A/1").as_int() );
    assert( g[3].open_string_note().as_int() ==
            Note("D/1").as_int() );

    assert( g.get_string(0).open_string_note().as_int() ==
            Note("G/2").as_int() );
    assert( g.get_string(1).open_string_note().as_int() ==
            Note("D/2").as_int() );
    assert( g.get_string(2).open_string_note().as_int() ==
            Note("A/1").as_int() );
    assert( g.get_string(3).open_string_note().as_int() ==
            Note("D/1").as_int() );

    g[3] = g[0]
    assert( g[3].open_string_note().as_int() ==
            Note("G/2").as_int() );

    print( "Passed Guitar Tests" )


def run_Measure_unit_tests_5():
    # Block 5: Overlapping notes
    m = Measure({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.25 ),
	MeasureNote( "E/7",  0.25, 0.125 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
    })

    #for i in range( 0, len(m) ):
    #    print( i, m[i].note )

    assert( len(m) == 4 )

    rests5 = m.compute_rests()
    assert( len(rests5) == 1 )

    rest5 = list(rests5)[0]
    assert( rest5.starting_point == 0.5 );
    assert( rest5.length == 0.25 );
    assert( rest5.ending_point() == 0.75 );


    # Block 5: Auto-assign strings
    g = GuitarFactory.standard_guitar()
    m = Measure({
	MeasureNote( Note("G/3")+3, g, 0.00, 0.25 ),
        MeasureNote( Note("A/2"),   g, 0.25, 0.25 ),
	MeasureNote( Note("D/3")-1, g, 0.75, 0.25 )
    })
    assert( m[0].string_assignment == 2 )
    assert( m[1].string_assignment == 4 )
    assert( m[2].string_assignment == 4 )

def run_Measure_unit_tests():
    # Block 1
    m = Measure({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.50 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
    })

    assert( m.size() == 3 );
    assert( m[0].starting_point == 0.0 );
    assert( m[1].starting_point == 0.25 );
    assert( m[2].starting_point == 0.75 );
    assert( m[0].length == 0.25 );
    assert( m[1].length == 0.50 );
    assert( m[2].length == 0.25 );
    assert( m[0].note == Note("Ab/5") );
    assert( m[1].note == Note( "E/4") );
    assert( m[2].note == Note("Gb/6") );

    rests = m.compute_rests()
    assert( len(rests) == 0 )


    # Block 2: Out of order
    m = Measure({
	MeasureNote( "E/4",  0.25, 0.50 ),
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
    })

    assert( m.size() == 3 );
    assert( m[0].starting_point == 0.0 );
    assert( m[1].starting_point == 0.25 );
    assert( m[2].starting_point == 0.75 );


    # Block 3: Single rest
    m = Measure({
	MeasureNote( "Ab/5", 0.00, 0.25 ),
	MeasureNote( "E/4",  0.25, 0.25 ),
	MeasureNote( "Gb/6", 0.75, 0.25 )
    })
    assert( m.size() == 3 );

    rests = m.compute_rests()
    assert( len(rests) == 1 )

    rest = list(rests)[0]
    assert( rest.starting_point == 0.5 );
    assert( rest.length == 0.25 );
    assert( rest.ending_point() == 0.75 );


    # Block 4: Multiple rest
    m = Measure({
	MeasureNote( "Ab/5", 0.25, 0.125 ),
	MeasureNote( "Gb/6", 0.75, 0.125 )
    })
    assert( m.size() == 2 );

    rests = sorted( m.compute_rests() )
    assert( len(rests) == 3 )

    rest = list(rests)[0]
    assert( rest.starting_point == 0.0 );
    assert( rest.length == 0.25 );
    assert( rest.ending_point() == 0.25 );

    rest = list(rests)[1]
    assert( rest.starting_point == 0.375 );
    assert( rest.length == 0.375 );
    assert( rest.ending_point() == 0.75 );

    rest = list(rests)[2]
    assert( rest.starting_point == 0.875 );
    assert( rest.length == 0.125 );
    assert( rest.ending_point() == 1.0 );


    # Offloading because it tends to fail more often
    # Giving it space to debug
    run_Measure_unit_tests_5()

    print( "Passed Measure Tests" )

if __name__ == '__main__':
    run_Note_unit_tests()
    run_GtrString_unit_tests()
    run_Guitar_unit_tests()
    run_Measure_unit_tests()
