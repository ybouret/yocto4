#include <R.h>

extern "C" void foo( )
{
	Rprintf("Foo!\n");
}

extern "C" void hello(int *n)
{
	int i;
  	for(i=0; i < *n; i++) {
    	Rprintf("Hello, world!\n");
	} 
}
