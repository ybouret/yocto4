all:

wrap:
	swig -r -outcurrentdir ../example.i

mac: wrap
	PKG_LIBS="../example.c" R CMD SHLIB example_wrap.c -o example.so
	R --no-save < main.R

clean:
	rm -f example.R example.so .RData example_wrap.*
