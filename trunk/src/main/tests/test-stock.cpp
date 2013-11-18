#include "yocto/utest/run.hpp"
#include "yocto/stock/stack.hpp"
#include "yocto/stock/queue.hpp"
#include "yocto/memory/pooled.hpp"
#include "support.hpp"

using namespace yocto;

namespace 
{
	
	template <typename STOCK>
	static inline void test_stock()
	{
		typedef typename STOCK::mutable_type type;
		STOCK S;
		for( size_t i=0; i < 2048; ++i )
		{
			type obj( gen<type>::get() );
			S.push( obj );
		}
		
		while( S.size() )
		{
			(void) S.peek();
			S.pop();
		}
		
	}
	
}

YOCTO_UNIT_TEST_IMPL(stock)
{
	test_stock< stack<int,vector<int> > >();
	test_stock< stack<string,vector<string,memory::pooled::allocator> > >();
	test_stock< queue<int,vector<int> > >();
	test_stock< queue<string,list<string> > >();

}
YOCTO_UNIT_TEST_DONE()
