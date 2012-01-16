#include "yocto/utest/run.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"

#include "yocto/codec/copy.hpp"
#include "yocto/codec/lzy.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/shared-ptr.hpp"

using namespace yocto;

namespace  
{
	
	class out_wrapper
	{
	public:
		inline out_wrapper( const string &id ) :
		fp( id + ".dat", false ) {}
		
		inline ~out_wrapper() throw() {}
		
		ios::ocstream fp;
		
		void write( char C )
		{
			
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(out_wrapper);
	};
	
	
}

using namespace codec;

#define __CODEC(NAME,KIND) \
out_wrapper   NAME##_##KIND##_wrapper( NAME::KIND::ID ); \
codec::writer NAME##_##KIND##_writer( & NAME##_##KIND##_wrapper, & out_wrapper:: write );\
NAME::KIND    NAME##_##KIND( NAME##_##KIND##_writer )

YOCTO_UNIT_TEST_IMPL(codecs)
{
	
	__CODEC(copy,encoder);
	__CODEC(lzy,encoder);
	
	codec::filter *filter_reg[] =
	{ 
		&copy_encoder,
		&lzy_encoder
	};
	
	const size_t filter_num = sizeof(filter_reg) / sizeof(filter_reg[0]);
	
	ios::icstream input(ios::cstdin);
	char C = 0;
	while( input.query(C) )
	{
		for( size_t i=0; i<filter_num; ++i )
		{
			filter_reg[i]->write(C);
		}
	}

	for( size_t i=0; i<filter_num; ++i )
	{
		filter_reg[i]->flush();
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
