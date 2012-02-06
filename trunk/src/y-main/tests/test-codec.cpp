#include "yocto/utest/run.hpp"
#include "yocto/code/bwt.hpp"
#include "yocto/code/mtf.hpp"

#include "yocto/string.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/ios/icstream.hpp"

using namespace yocto;



YOCTO_UNIT_TEST_IMPL(codec)
{

	core::move_to_front mtf_encoder;
	core::move_to_front mtf_decoder;
	
	string         line;
	vector<size_t> indices;
	ios::icstream  fp( ios::cstdin );
	
	while( fp.read_line(line) > 0 )
	{
		const size_t size = line.size();
		string       ostr = line;
		indices.make( size, 0 );
		const size_t pidx = core::bwt::encode( ostr.rw(), line.ro(), size, size ? &indices[1] : NULL);
		std::cerr << "src: [" << line << "]"   << std::endl;
		std::cerr << "dst: [" << ostr << "] @" << pidx << std::endl; 
		
		string enc;
		std::cerr << "enc: [";
		for( size_t i=0; i < size; ++i )
		{
 			const char C = mtf_encoder.encode( ostr[i] );
			enc.append( C );
			std::cerr << make_visible(C);
		}
		std::cerr << "]" << std::endl;
		string dec;
		for( size_t i=0; i < size; ++i )
		{
			dec.append( mtf_decoder.decode( enc[i] ) );
		}
		std::cerr << "dec: [" << dec << "]" << std::endl; 

		
		
		string       fstr = ostr;
		core::bwt::decode( fstr.rw(), ostr.ro(), size, size ? &indices[1] : NULL, pidx );
		std::cerr << "org: [" << fstr << "]"   << std::endl;
		line.clear();
	}
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
