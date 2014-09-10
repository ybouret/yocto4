#include "yocto/utest/run.hpp"
#include "yocto/string/utf8.hpp"
#include <cstdlib>
#include <cstdio>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(utf8)
{
	std::cerr << "Testing All " << unsigned(1<<21) << std::endl;
	std::cerr.flush();
	
	string out;
	for( utf8_t i=0; i < (1<<21); ++i )
	{
		out.clear();
		UTF8::Encode( i, out);
		const char *curr = out.c_str();
		const char *last = curr + out.size();
		size_t nr = 0;
		while( curr < last )
		{
			++nr;
			if( nr > 1 )
				throw exception("Invalid encoding of 0x%08x", unsigned(i));
			const utf8_t U = UTF8::Decode( curr, last );
			if( U !=i )
				throw exception("Invalid decoding of 0x%08x", unsigned(i));
		}
		
	}
	
	static const utf8_t cp[] =
	{
		0x0000,
		0x001F,
		0x0020,
		0x0041,
		0x007E,
		0x007F,
		0x0080,
		0x009F,
		0x00A0,
		0x00E9,
		0x07FF,
		0x0800,
		0x20AC,
		0xD7FF,
		0x10000,
		0x1D11E
	};
	
	std::cerr << "Testing Selected" << std::endl;
	std::cerr.flush();
	
	for( size_t i=0; i < sizeof(cp)/sizeof(cp[0]); ++i )
	{
		out.clear();
		const utf8_t U = cp[i];
		if(UTF8::Encode( U, out))
            throw exception("invalid UTF8 value");
		fprintf( stderr, "0x%8x =>", unsigned(U) );
		for( size_t j=0; j < out.size(); ++j )
		{
			const uint8_t b = out[j];
			fprintf(stderr, " %02X", b );
		}
		fprintf( stderr, "\n" );
	}
	fflush( stderr );
	
	std::cerr << "Testing Random" << std::endl;
	std::cerr.flush();
	
	for( size_t k=0; k < 1024; ++k )
	{
		wstring w;
		for( size_t j = 32 + ( rand() & 1023 ); j>0; --j )
		{
			w.append( ( rand() ) % ( 1 << 21 ) );
		}
		const string   s = UTF8::Encode( w );
		const wstring  z = UTF8::Decode( s );
		if( z != w )
		{
			throw exception("Full Conversion Failure!");
		}
	}
	
}
YOCTO_UNIT_TEST_DONE()

