#include "yocto/core/pool.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace 
{
	struct pnode
	{
		pnode  *next;
		size_t  data;
	};
	
	static inline void display( pnode *node) throw()
	{
		std::cerr << node->data << std::endl;
	}
}

YOCTO_UNIT_TEST_IMPL(pool)
{
	const size_t         num   = 10;
	pnode               *nodes = new pnode[ num ];
	core::pool_of<pnode> pool;
	
	for( size_t i=0; i < num; ++i )
	{
		nodes[i].next = NULL;
		nodes[i].data = i;
	}
	c_shuffle( nodes, num);
	
	for( size_t i=0; i < num; ++i )
	{
		pool.store( nodes+i );
	}
	
	pool.delete_with( display );
	
	
	delete [] nodes;
}
YOCTO_UNIT_TEST_DONE()
