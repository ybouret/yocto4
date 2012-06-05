all:

wrap:
	swig -csharp -outcurrentdir ../example.i

mac: wrap
	gcc -shared -o libexample.so *.c ../example.c -Wall -O3 -fPIC
	mcs -out:runme *.cs
	mono runme

linux: mac

clean:
	rm -f example_wrap.c libexample.so example.cs examplePINVOKE.cs runme runme.exe
