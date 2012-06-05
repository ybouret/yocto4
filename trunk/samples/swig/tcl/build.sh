# must use /usr/bin/tclsh
swig -tcl -outcurrentdir ../example.i
gcc -dynamiclib -o example.so ../example.c example_wrap.c -framework Tcl

