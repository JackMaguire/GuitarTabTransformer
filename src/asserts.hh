#pragma once

#include <exception>
#include <string>

struct GuitarTabTransformerException : public std::exception {

  std::string message_ = "GuitarTabTransformerException";

  GuitarTabTransformerException( std::string const & message ) :
    message_( message )
  {}

  char const * what() const throw() {
    return message_.c_str();
  }

};

//#define MY_ASSERT(x) if (!(x)) { throw(  STR(x), __PRETTY_FUNCTION__, __FILE__, __LINE__); abort(); }
//#define MY_ASSERT(x) if (!(x)) { printf("My custom assertion failed: (%s), function %s, file %s, line %d.\n", STR(x), __PRETTY_FUNCTION__, __FILE__, __LINE__); abort(); }
#define STR(x) #x
#define GTT_ASSERT(x) if (!(x)) { throw GuitarTabTransformerException( "'" + std::string(STR(x)) + "' failed on line " + std::to_string( __LINE__ ) + " in " + __FILE__ ); }

#define GTT_ASSERT_EQUALS(x,y) if (!(x == y)) { throw GuitarTabTransformerException( std::to_string(x) + " == " + std::to_string(y) + " failed on line " + std::to_string( __LINE__ ) + " in " + __FILE__ ); }

#define GTT_ASSERT_DELTA( x , y, d ) if ( std::abs( x - y ) > d ) { throw GuitarTabTransformerException( std::to_string(x) + " is within " + std::to_string(d) + " of " + std::to_string(y) + " failed on line " + std::to_string( __LINE__ ) + " in " + __FILE__ ); }

#ifndef GTT_DEBUG
#define GTT_DEBUG_ASSERT(condition) ((void)0)
#define GTT_DEBUG_ASSERT_EQUALS(x,y) ((void)0)
#define GTT_DEBUG_ASSERT_DELTA(x,y,d) ((void)0)
#else
#define GTT_DEBUG_ASSERT(condition) GTT_ASSERT( condition )
#define GTT_DEBUG_ASSERT_EQUALS(x,y) GTT_ASSERT_EQUALS(x,y)
#define GTT_DEBUG_ASSERT_DELTA(x,y,d) GTT_ASSERT_DELTA(x,y,d)
#endif
