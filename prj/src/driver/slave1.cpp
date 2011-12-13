#include "yocto/wtime.hpp"

#include <cstdio>

using namespace yocto;

int main()
{
		
	wtime chrono;
	
	chrono.start();
	size_t index = 0;
	fprintf( stderr, "Running for 10 seconds...\n");
	while( chrono.query() <= 10.0 )
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

