#include "yocto/utest/run.hpp"
#include "yocto/pack/bwt.hpp"
#include "yocto/pack/mtf.hpp"

#include "yocto/string.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/ios/icstream.hpp"

using namespace yocto;

static inline void display( const string &str )
{
    std::cerr.flush();
    fflush(stderr);
    for( size_t i=0; i < str.length(); ++i )
    {
        const char C = str[i];
        if( C >= 32 && C < 127 )
        {
            fprintf( stderr, "<%c>",C );
        }
        else 
        {
            fprintf( stderr, "x%02x", C );
        }
    }
    fprintf( stderr, "\n" );
    fflush(stderr);
    std::cerr.flush();
}

YOCTO_UNIT_TEST_IMPL(codec)
{
    
	pack::move_to_front mtf_encoder;
	pack::move_to_front mtf_decoder;
	
	string         line;
	vector<size_t> indices;
	ios::icstream  fp( ios::cstdin );
	
	while( fp.read_line(line) > 0 )
	{
		const size_t size = line.size();
		string       ostr = line;
		indices.make( size, 0 );
		const size_t pidx = pack::bwt::encode( ostr.rw(), line.ro(), size, size ? &indices[1] : NULL);
		std::cerr << "src: "; display( line );
		std::cerr << "dst: "; display( ostr );
		
		string enc;
		std::cerr << "enc: ";
		for( size_t i=0; i < size; ++i )
		{
 			const char C = mtf_encoder.encode( ostr[i] );
			enc.append( C );
		}
        display( enc );
		string dec;
		for( size_t i=0; i < size; ++i )
		{
			dec.append( mtf_decoder.decode( enc[i] ) );
		}
		std::cerr << "dec: "; display( dec );
        
		
		
		string       fstr = ostr;
		pack::bwt::decode( fstr.rw(), ostr.ro(), size, size ? &indices[1] : NULL, pidx );
		std::cerr << "org: "; display( fstr );
		line.clear();
	}
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
