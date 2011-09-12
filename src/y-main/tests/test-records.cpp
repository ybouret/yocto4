#include "yocto/utest/run.hpp"
#include "yocto/memory/records.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/memory/global.hpp"

using namespace yocto;

namespace {
	
	class dummy 
	{
	public:
		int value;
		
		inline dummy( int a ) : value(a) { std::cerr << "+[" << value << "]" << std::endl; }
		inline ~dummy() throw() { std::cerr << "-[" << value << "]" << std::endl; }
		
		static inline void build( void *p, void *args )
		{
			assert( p != NULL );
			assert( args != NULL );
			size_t a = *(size_t *)args;
			new (p) dummy(a);
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
	
	
}

template <typename BUFF>
static inline void test_rec( BUFF &buff )
{
	memory::records_of<dummy> rec( buff(), buff.size );

	std::cerr << "---- build records" << std::endl;
	for( size_t i=0; i < buff.size; ++i )
	{
		rec( dummy::build, &i );
	}
	std::cerr << "---- done" << std::endl;
}


YOCTO_UNIT_TEST_IMPL(records)
{
	memory::buffer_of<dummy,memory::global> gbuff(10);
	test_rec( gbuff );
	
	memory::buffer_of<dummy,memory::pooled> pbuff(12);
	test_rec( pbuff );
}
YOCTO_UNIT_TEST_DONE()
