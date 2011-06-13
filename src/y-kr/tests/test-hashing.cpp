#include "yocto/utest/run.hpp"
#include "yocto/hashing/md2.hpp"
#include "yocto/hashing/md4.hpp"
#include "yocto/hashing/md5.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/hashing/digest.hpp"

#include <iomanip>

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
		std::cerr << std::setw(16) << h_reg[i]->name() << " : " << h_reg[i]->length << " | " << h_reg[i]->window << std::endl;
	}
	vector<string> words;
	
	if( argc > 1 )
	{
		//-- Get Global Hash
		{
			ios::icstream input( argv[1] );
			for( size_t i=0; i < h_num; ++i )
			{
				h_reg[i]->set();
			}
			char   buf[512];
			size_t len = 0;
			while( (len = input.get(buf, sizeof(buf) ) ) > 0  )
			{
				for( size_t i=0; i < h_num; ++i )
				{
					h_reg[i]->run(buf,len);
				}
			}
			for( size_t i=0; i < h_num; ++i )
			{
				const digest D = digest::get_from(*h_reg[i]);
				std::cerr << std::setw(16) << h_reg[i]->name() << " : " << D << std::endl;
			}
		}
		
		//-- extract words
		{
			ios::icstream input( argv[1] );
			string line;
			while( input.read_line(line) >= 0 )
			{
				words.push_back(line);
				line.clear();
			}
		}
	}
	else
	{
		for( size_t i=0; i < 256; ++i )
		{
			const string tmp = char(i);
			words.push_back(tmp);
		}
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
