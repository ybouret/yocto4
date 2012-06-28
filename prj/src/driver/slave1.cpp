#include "yocto/wtime.hpp"

#include <cstdio>
#include <cstdlib>

using namespace yocto;

int main(int argc, char *argv[])
{
		
	wtime chrono;
    double tmax = 10.0;
	if( argc > 1 )
        tmax = atof(argv[1]);
    
	chrono.start();
	size_t index = 0;
	fprintf( stderr, "Running for %g seconds...\n", tmax);
	while( chrono.query() <= tmax )
	{
		putc( char( 'A' + index++ ), stdout );
		if( index >= 26 ) 
		{
			putc('\n',stdout);
			index = 0;
		}
		fflush( stdout );
		wtime::sleep( 0.1 );
		
	}
	putc('\n',stdout);
	return 0;
}

