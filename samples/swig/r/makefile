all:

wrap:
	swig -r -outcurrentdir ../example.i

mac: wrap
	R CMD SHLIB example_wrap.c ../example.c -o example.so
	R --no-save < main.R

linux: mac

openbsd: mac


clean:
	rm -f example.R example.so .RData example_wrap.*
