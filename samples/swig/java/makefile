all:

wrap:
	swig -java  -outcurrentdir ../example.i
	javac main.java

mac: wrap
	gcc -shared -o libexample.jnilib ../example.c  example_wrap.c -I`javaconfig Headers` -framework JavaVM -O3 -Wall
			
clean:
	rm -f *.class example.java exampleJNI.java example_wrap.c libexample.jnilib
