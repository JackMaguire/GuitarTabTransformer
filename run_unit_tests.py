from gtt import *
import gtt.shortcuts as X

def run_X_unit_tests():
    # Block 1
    n = Note( "G/4" )

    print( "Passed X Tests" )


def run_note_unit_tests():
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

if __name__ == '__main__':
    run_note_unit_tests()
    run_GtrString_unit_tests()
    run_Guitar_unit_tests()
