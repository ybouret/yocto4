all:

wrap:
	swig -java  -outcurrentdir ../example.i
	javac main.java

mac: wrap
	gcc -shared -o libexample.jnilib ../example.c example_wrap.c -I`javaconfig Headers` -framework JavaVM -O3 -Wall
	java main

linux: wrap
	gcc -fPIC -shared -o libexample.so ../example.c example_wrap.c -I${JDK_HOME}/include -I${JDK_HOME}/include/linux
	java -Djava.library.path=. main
			
clean:
	rm -f *.class example.java exampleJNI.java example_wrap.c libexample.jnilib libexample.so
