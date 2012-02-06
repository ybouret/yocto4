#include "yocto/utest/run.hpp"
#include "yocto/code/bwt.hpp"
#include "yocto/string.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/ios/icstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(codec)
{

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
		string       fstr = ostr;
		core::bwt::decode( fstr.rw(), ostr.ro(), size, size ? &indices[1] : NULL, pidx );
		std::cerr << "dec: [" << fstr << "]"   << std::endl;
		line.clear();
	}
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
