#include "yocto/utest/run.hpp"
#include "yocto/hashing/md2.hpp"
#include "yocto/hashing/md4.hpp"
#include "yocto/hashing/md5.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(hashing)
{
	hashing::md2 h_md2;
	hashing::md4 h_md4;
	hashing::md5 h_md5;

	hashing::function * h_reg[] = { &h_md2, &h_md4, &h_md5 };
	const size_t        h_num   = sizeof(h_reg)/sizeof(h_reg[0]);

	for( size_t i=0; i < h_num; ++i )
	{
		std::cerr << h_reg[i]->name() << std::endl;
	}
	vector<string> words;
	
	if( argc > 1 )
	{
		ios::icstream input( argv[1] );
		string line;
		while( input.read_line(line) >= 0 )
		{
			words.push_back(line);
			std::cerr << line << std::endl;
			line.clear();
		}
	}
	else
	{
		
	}


}
YOCTO_UNIT_TEST_DONE()
