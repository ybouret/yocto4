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
	
	
	for( size_t block_size=1; block_size<=300; block_size += 1 + alea_leq(30) )
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
	
	for( size_t bs=0; bs<=1024; bs += 1 + alea_leq(100) )
	{
		fragment::block B( bs );
		std::cerr << std::dec << "length = " << B.length() << " | unused=" << B.unused() << std::endl;
		H.set();
		while( B.unused() > 0 )
		{
			char buffer[32];
			const size_t buflen = 1+alea_lt( sizeof(buffer) );
			for( size_t j=0; j < buflen; ++j ) buffer[j] = char('A' + alea_lt(26) );
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
			const size_t buflen = 1+alea_lt( sizeof(buffer) );
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

#include "yocto/fragment/queue.hpp"

YOCTO_UNIT_TEST_IMPL(frag_queue)
{

	hashing::sha1 H;
	const size_t size = 16 * 1024;
	uint8_t     *huge = new uint8_t[ size ];
	for( size_t i=0; i < size; ++i ) huge[i] = alea_of<uint8_t>();
	
	H.set();
	H.run(huge,size);
	H.run(huge,size);
	const uint64_t k0 = H.key<uint64_t>();
	
	std::cerr << "k0=" << k0 << std::endl;
	
	try
	{
		for( size_t bs=10; bs < 1000; bs += 1 + alea_leq(100) )
		{
			fragment::queue Q(bs);
			std::cerr << "block_size=" << Q.block_size << std::endl;
			size_t done = 0;
			Q.put( huge, size, done );
			
			for(size_t i=0; i < size; ++i )
			{
				Q.write( huge[i] );
				++done;
			}
			
			if( Q.bytes() != done ) throw exception("invalid Q.bytes");
			
			{
				H.set();
				char   buffer[512];
				while( Q.bytes() )
				{
					const size_t buflen = Q.get( buffer, 1+alea_lt(sizeof(buffer)) );
					//std::cerr << "+" << buflen << std::endl;
					H.run( buffer, buflen );
				}
				
				const uint64_t k1 = H.key<uint64_t>();
				std::cerr << "k1=" << k1 << std::endl;
				
			}
			
		}
	}
	catch(...)
	{
		delete [] huge; throw;
	}
	delete [] huge;
}
YOCTO_UNIT_TEST_DONE()

