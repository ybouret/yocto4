#include "yocto/utest/run.hpp"
#include "yocto/fragment/layout.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

static inline void frag_cb( void *frag_data, size_t frag_size, void *args )
{
	size_t count = *(size_t *)args;
	++count;
	std::cerr << frag_size << "/";
}


YOCTO_UNIT_TEST_IMPL(frag_layout)
{

	
	for( size_t block_size=1; block_size<=300; block_size += 1 + alea_less_than<size_t>(30) )
	{
		std::cerr << "block_size=" << block_size << std::endl;
		for( size_t ln2 = fragment::layout::round_log2_min; ln2 <= fragment::layout::round_log2_max; ++ln2 )
		{
			fragment::layout L( block_size, ln2 );
			std::cerr << "\tln2=" << ln2 << " => block_size=" << L.block_size << std::endl;
			size_t count = 0;
			L.process( NULL, 3000, frag_cb, &count );
			std::cerr << std::endl;
		}
	}
	
}
YOCTO_UNIT_TEST_DONE()
