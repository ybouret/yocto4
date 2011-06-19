#include "yocto/utest/run.hpp"
#include "yocto/associative/ktable.hpp"
#include "yocto/memory/global.hpp"

using namespace yocto;

namespace 
{
	class dummy
	{
	public:
		dummy *next;
		dummy *prev;
		int    a;
		
		dummy() : next(NULL), prev(NULL), a(0) {}
		~dummy() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
}

YOCTO_UNIT_TEST_IMPL(ktable)
{
	
	memory::global::allocator alloc;
	
	{ core::ktable<dummy> tab; }
	
	for( size_t i=0; i <= 30; ++i )
	{
		core::ktable<dummy> tab(i,alloc);
		std::cerr << "ktable #" << i << " : #nodes=" << tab.nodes << ", #slots=" << tab.count << " and " << tab.num_bytes() << " bytes" << std::endl;
		if( tab.nodes != tab.cache.available() )
		{
			throw exception("ktable/cache mismatch!");
		}
		
		if( tab.nodes != tab.kpool.available() )
		{
			throw exception("ktable/kpool mismatch!");
		}
		
		for( size_t j=0; j < tab.nodes; ++j )
		{
			
		}
		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
