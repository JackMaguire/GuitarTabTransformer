#!/bin/bash

#find ../wt/build/src/ | grep "\.so" | xargs -n1 ln -s
#ln -s ../wt/build/src/http/libwthttp.so libwthttp2.so
#ln -s ../wt/build/src/libwt.so libwt2
#ln -s ../wt/resources/

WARN="-Wall -Wshadow -Wunused -pedantic -Wextra -Werror"
WARN="$WARN -Wno-subobject-linkage"

DEBUG="-D_GLIBCXX_DEBUG"
#DEBUG="$DEBUG -g"

opt="-O3"

include="$include -I ."
include="$include -I extern/json/single_include/"
include="$include -isystem extern/wt/src"

libraries="$libraries -lwt"
libraries="$libraries -lwthttp"
libraries="$libraries -Lextern/wt/build/src"
libraries="$libraries -Lextern/wt/build/src/http"
libraries="$libraries -L/usr/lib/x86_64-linux-gnu/"

cmd="g++-11 --std=c++2a -o run_unit_tests run_unit_tests.cc $opt $libraries $include -Wl,-rpath,. $WARN $DEBUG"
echo $cmd
$cmd
