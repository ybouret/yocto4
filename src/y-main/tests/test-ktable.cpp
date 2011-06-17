#include "yocto/utest/run.hpp"
#include "yocto/core/ktable.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/memory/pooled.hpp"

#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ktable)
{
	std::cerr << "sizeof(core::kslot)=" << sizeof(core::kslot) << std::endl;
	std::cerr << "sizeof(core::knode)=" << sizeof(core::knode) << std::endl;
	std::cerr << "sizeof(core::ktable)=" << sizeof(core::ktable) << std::endl;
	
	memory::global::allocator alloc;
	memory::pooled::allocator alloc2;
	
	core::ktable tab0;
	tab0.release(alloc);
	
	for( size_t i=0; i<=100;++i)
	{
		core::ktable tab(i,alloc);
		assert( tab.available() >= i );
		for( size_t j=tab.available(); j>0; --j )
		{
			tab.insert( alea_of<size_t>(), NULL );
		}
		
		try
		{
			core::ktable tab2(i,alloc2);
			tab.move_to( tab2 );
			
			tab2.release(alloc2);
		}
		catch(...)
		{
			std::cerr << "Unable to allocate tab2("<< i <<")" << std::endl;
		}
		tab.release(alloc);
	}
	
}
YOCTO_UNIT_TEST_DONE()

