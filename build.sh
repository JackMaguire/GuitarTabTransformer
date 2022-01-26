#!/bin/bash

#find ../wt/build/src/ | grep "\.so" | xargs -n1 ln -s
#ln -s ../wt/build/src/http/libwthttp.so libwthttp2.so
#ln -s ../wt/build/src/libwt.so libwt2
#ln -s ../wt/resources/

WARN="-Wall -Wshadow -Wunused -pedantic -Wextra -Werror"
#WARN="$WARN -Wno-subobject-linkage"

#DEBUG="-D_GLIBCXX_DEBUG"
#DEBUG="$DEBUG -g"

#include="$include -I /root/wt_util/include"
include="$include -I ."
include="$include -I extern/json/single_include/"
#include="$include -isystem tf/include"
#include="$include -isystem cppflow/include"
#include="$include -isystem wt_build_src"
include="$include -isystem extern/wt/src"

libraries="$libraries -lwt"
libraries="$libraries -lwthttp"
#libraries="$libraries -ltensorflow"
#libraries="$libraries -lboost_signals"
#libraries="$libraries -lstdc++fs"
#libraries="$libraries -L."
#libraries="$libraries -Ltf/lib"
libraries="$libraries -Lextern/wt/build/src"
libraries="$libraries -Lextern/wt/build/src/http"
libraries="$libraries -L/usr/lib/x86_64-linux-gnu/"

opt="-O3"

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/tf/lib/

#clang++ --stdlib=libc++

cmd="g++-11 --std=c++2a -o run_unit_tests run_unit_tests.cc $opt $libraries $include -Wl,-rpath,. $WARN $DEBUG"
echo $cmd
$cmd
#g++-11 --std=c++2a -o run_unit_tests run_unit_tests.cc $opt $libraries $include -Wl,-rpath,. $WARN $DEBUG
