all:

wrap:
	swig -csharp -outcurrentdir ../example.i

# can run on linux, depends on mono version
mac: wrap
	gcc -shared -o libexample.so *.c ../example.c -Wall -O3 -fPIC
	mcs -out:runme *.cs
	mono runme

linux: wrap
	gcc -shared -o libexample.so *.c ../example.c -Wall -O3 -fPIC
	mono-csc -out:runme.exe *.cs
	./runme.exe

openbsd: mac 

clean:
	rm -f example_wrap.c libexample.so example.cs examplePINVOKE.cs runme runme.exe
