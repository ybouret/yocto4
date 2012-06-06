all:

wrap:
	swig -python -outcurrentdir ../example.i

mac: wrap
	gcc -shared -o _example.so ../example.c  example_wrap.c `python-config --cflags --ldflags` -Wall -O3 -fPIC
	python main.py
	
linux: mac

openbsd: wrap
	gcc -shared -o _example.so ../example.c  example_wrap.c `python2.7-config --cflags --ldflags` -Wall -O3 -L/usr/local/lib
	python2.7 main.py

clean:
	rm -f example.py* example_wrap.c _example.so


