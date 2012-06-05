all:

wrap:
	swig -tcl -outcurrentdir ../example.i

mac: wrap
	gcc -shared -o example.so ../example.c example_wrap.c -framework Tcl -Wall -O3
	/usr/bin/tclsh main.tcl

linux: wrap
	gcc -shared -o example.so ../example.c example_wrap.c -ltcl -fPIC -Wall -O3 -I/usr/include/tcl
	tclsh main.tcl
	
clean:
	rm -f example.so example_wrap.c
