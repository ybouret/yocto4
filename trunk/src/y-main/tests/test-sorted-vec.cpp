#include "yocto/utest/run.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "support.hpp"
#include "yocto/code/utils.hpp"

#include <typeinfo>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(sorted_vec)
{
	
	{
		sorted_vector<size_t> sv;
		size_t count = 0;
		const size_t nmax = 1000;
		for( size_t i=0; i < nmax; ++i)
		{
			if( sv.insert( alea_lt(nmax) ) )
			{
				++count;
			}
		}
		std::cerr << "inserted " << count << "/" << nmax << std::endl;
		assert( sv.size() == count );
		const size_t step = max_of<size_t>(1,count/10);
		for( size_t i=1; i <= count; i += step )
		{
			std::cerr << sv[i] << std::endl;
		}
		sorted_vector<size_t> sv2( sv );
		sv2.free();
	}
	
	{
		sorted_vector<string> sv;
		size_t count = 0;
		const size_t nmax = 1000;
		for( size_t i=0; i < nmax; ++i)
		{
			const string tmp = gen<string>::get();
			if( sv.insert( tmp ) )
			{
				++count;
			}
		}
		std::cerr << "inserted " << count << "/" << nmax << std::endl;
		assert( sv.size() == count );
		const size_t step = max_of<size_t>(1,count/10);
		for( size_t i=1; i <= count; i += step )
		{
			std::cerr << sv[i] << std::endl;
		}
		sorted_vector<string> sv2( sv );
		sv2.free();
	}
}
YOCTO_UNIT_TEST_DONE()
