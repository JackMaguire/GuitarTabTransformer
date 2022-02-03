#!/bin/bash

app_name=$1

#find ../wt/build/src/ | grep "\.so" | xargs -n1 ln -s
#ln -s ../wt/build/src/http/libwthttp.so libwthttp2.so
#ln -s ../wt/build/src/libwt.so libwt2
#ln -s ../wt/resources/

WARN="-Wall -Wshadow -Wunused -pedantic -Wextra"
#WARN="$WARN -Wno-subobject-linkage"

#DEBUG="-D_GLIBCXX_DEBUG"
#DEBUG="$DEBUG -g"

#opt="-O0"
opt="-O3"

include="$include -I ."
include="$include -I extern/json/single_include/"
include="$include -isystem extern/wt/src/"
include="$include -isystem extern/wt/build/"

libraries="$libraries -lwt"
libraries="$libraries -lwthttp"
libraries="$libraries -Lextern/wt/build/src"
libraries="$libraries -Lextern/wt/build/src/http"
libraries="$libraries -L/usr/lib/x86_64-linux-gnu/"
libraries="$libraries -L."

export LD_LIBRARY_PATH=".:extern/wt/build/src:extern/wt/build/src/http:/usr/lib/x86_64-linux-gnu/"

mkdir bin 2>/dev/null
cmd="g++-11 --std=c++2a -o bin/$app_name apps/${app_name}.cc $opt $libraries $include -Wl,-rpath,. $WARN $DEBUG -fPIC"
echo $cmd
$cmd

echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
