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
	const size_t num   = 30 + alea_leq(100);
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

#include "yocto/core/clist.hpp"
YOCTO_UNIT_TEST_IMPL(clist)
{
    
    for( size_t iter =1 ; iter <= 1024; ++iter )
    {
        const size_t num   = 20 + alea_leq(20);
        node_type   *nodes = new node_type[num];
        core::clist_of<node_type> L;
        
        
        for( size_t i=0; i < num; ++i )
        {
            nodes[i].next = NULL;
            nodes[i].prev = NULL;
            nodes[i].data = i;
        }
        
        for( size_t i=0; i < num; ++i )
        {
            if( alea<float>() > 0.5f )
            {
                L.push_back(nodes+i);
            }
            else 
            {
                L.push_front(nodes+i);
            }
            node_type *node = L.root;
            for( size_t j=0; j < L.size; ++j, node=node->next )
            {
                std::cerr << " " << node->data;
            }
            if(node)
                std::cerr << "->" << node->data;
            std::cerr << std::endl;
        }
        
        while( L.size > 0 )
        {
            if( alea<float>() > 0.5f )
            {
                node_type *node = L.pop_back();
                std::cerr << "pop_back  = " << node->data << std::endl;
            }
            else 
            { 
                node_type *node = L.pop_front();
                std::cerr << "pop_front = " << node->data << std::endl;
            }
        }
        
        c_shuffle(nodes, num);
        for( size_t i=0; i < num; ++i )
        {
            L.push_back(nodes+i);
        }
        
        while( L.size > 0 )
        {
            node_type *node = L.fetch( alea_lt(L.size) );
            node_type *curr = L.unlink(node);
            assert(curr==node);
            std::cerr << "unlink = " << node->data << std::endl;
        }
        
        //L.reset();
        delete [] nodes;
    }
}
YOCTO_UNIT_TEST_DONE()
