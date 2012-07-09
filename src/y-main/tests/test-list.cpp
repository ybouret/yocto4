#include "yocto/core/list.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace  
{
	struct node_type
	{
		node_type *next, *prev;
		size_t     data;
	};
	
	static inline void display( node_type *node ) throw()
	{
		std::cerr << node->data << std::endl;
	}
}

YOCTO_UNIT_TEST_IMPL(list)
{
	const size_t num   = 30;
	node_type   *nodes = new node_type[num];
	core::list_of<node_type> L;
	
	for( size_t i=0; i < num; ++i )
	{
		nodes[i].next = NULL;
		nodes[i].prev = NULL;
		nodes[i].data = i;
	}
	
	for( size_t i=0; i < num; ++i )
	{
		node_type *node = nodes+i;
		if( alea<float>() < 0.5 )
		{
			L.push_front( node );
			std::cerr << ">";
		}
		else
		{
			L.push_back( node );
			std::cerr << "<";
		}
	}
	std::cerr << std::endl;
	std::cerr << "--" << std::endl;
	{
		size_t i=0;
		while( L.size > 0 )
		{
			node_type *node =  alea<float>() < 0.5 ? L.pop_front() : L.pop_back();
			std::cerr << ++i << " : " << node->data << std::endl;	
		}
	}
	
	for( size_t i=0; i < num; ++i )
	{
		node_type *node = nodes+i;
		if( alea<float>() < 0.5 )
		{
			L.push_front( node );
			std::cerr << ">";
		}
		else
		{
			L.push_back( node );
			std::cerr << "<";
		}
	}
	std::cerr << std::endl;
	std::cerr << "--" << std::endl;
	L.delete_with( display );
	
	
	
	for( size_t i=0; i < num; ++i )
	{
		node_type *node = nodes+i;
		if( alea<float>() < 0.5 )
		{
			L.push_front( node );
			std::cerr << ">";
		}
		else
		{
			L.push_back( node );
			std::cerr << "<";
		}
	}
	std::cerr << std::endl;
	std::cerr << "--" << std::endl;
	{
		size_t j=0;
		while( L.size )
		{
			node_type *node = L.unlink( L.fetch( alea_lt( L.size ) ) );
			std::cerr << ++j << " : " << node->data << std::endl;	
		}
	}
	
	for( size_t i=0; i < num; ++i )
	{
		node_type *node = nodes+i;
		if( alea<float>() < 0.5 )
		{
			L.push_front( node );
			std::cerr << ">";
		}
		else
		{
			L.push_back( node );
			std::cerr << "<";
		}
	}
	std::cerr << std::endl;
	std::cerr << "--" << std::endl;
	for( size_t i=0; i < 100 * num; ++i )
	{
		L.move_to_front( L.fetch( alea_lt( L.size ) ) );
	}
	L.reset();
	delete [] nodes;
}
YOCTO_UNIT_TEST_DONE()
