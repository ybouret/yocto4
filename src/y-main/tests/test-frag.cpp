#include "yocto/utest/run.hpp"
#include "yocto/fragment/layout.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/hashing/sha1.hpp"

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

#include "yocto/fragment/block.hpp"

YOCTO_UNIT_TEST_IMPL(frag_block)
{
	
	hashing::sha1 H;
	
	for( size_t bs=0; bs<=1024; bs += 1 + alea_less_than<size_t>(100) )
	{
		fragment::block B( bs );
		std::cerr << std::dec << "length = " << B.length() << " | unused=" << B.unused() << std::endl;
		H.set();
		while( B.unused() > 0 )
		{
			char buffer[32];
			const size_t buflen = 1+alea_less_than<size_t>( sizeof(buffer) );
			for( size_t j=0; j < buflen; ++j ) buffer[j] = char('A' + alea_less_than<size_t>(26) );
			const size_t len = B.write(buffer, buflen);
			std::cerr << ".";
			H.run(buffer,len);
		}
		std::cerr << std::endl;
		const uint64_t kw = H.key<uint64_t>();
		std::cerr << std::hex << "kw=" << kw << std::endl;
		
		H.set();
		while( B.length() > 0 )
		{
			char buffer[32];
			const size_t buflen = 1+alea_less_than<size_t>( sizeof(buffer) );
			const size_t len = B.read(buffer,buflen);
			H.run(buffer,len);
			if( alea<float>() > 0.66f ) 
			{
				B.defrag(); std::cerr << "*";
			}
			std::cerr << ".";
		}
		std::cerr << std::endl;
		const uint64_t kr = H.key<uint64_t>();
		std::cerr << std::hex << "kr=" << kr << std::endl;
		if( kr != kw )
			throw exception("block data mismatch");
	}
	
}
YOCTO_UNIT_TEST_DONE()
