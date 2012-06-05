all:

wrap:
	swig -python -outcurrentdir ../example.i

mac: wrap
	gcc -shared -o _example.so ../example.c  example_wrap.c `python-config --cflags --ldflags` -Wall -O3
	

clean:
	rm -f example.py* example_wrap.c _example.so


