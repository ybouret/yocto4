all:

wrap:
	swig -ruby -outcurrentdir ../example.i
mac: wrap
	gcc -shared ../example.c example_wrap.c -o example.so -I/opt/local/lib/ruby/1.8/i686-darwin11 -L/opt/local/lib -lruby -Wall -O2

clean:
	rm -f example_wrap.c example.so
