all:

wrap:
	swig -tcl -outcurrentdir ../example.i

mac: wrap
	gcc -shared -o example.so ../example.c example_wrap.c -framework Tcl
	
clean:
	rm -f example.so example_wrap.c
