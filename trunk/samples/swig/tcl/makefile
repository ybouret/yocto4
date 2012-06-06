all:

wrap:
	swig -tcl -outcurrentdir ../example.i

mac: wrap
	gcc -shared -o example.so ../example.c example_wrap.c -framework Tcl -Wall -O3
	/usr/bin/tclsh main.tcl

linux: wrap
	gcc -shared -o example.so ../example.c example_wrap.c -ltcl -fPIC -Wall -O3 -I/usr/include/tcl
	tclsh main.tcl

openbsd: wrap
	gcc -shared -o example.so ../example.c example_wrap.c -ltcl85 -fPIC -Wall -O3 -I/usr/local/include/tcl8.5 -L/usr/local/lib
	tclsh8.5 main.tcl
	
clean:
	rm -f example.so example_wrap.c
