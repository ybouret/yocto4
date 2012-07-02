all:
	MAKEFLAGS="CXXFLAGS=-O2" R CMD SHLIB foo.cpp
	R --no-save < foo.R

clean:
	rm -f *~ *.o *.so

