#include "yocto/core/list.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/comparator.hpp"
using namespace yocto;

namespace  
{
	class node_type
	{
	public:
		inline node_type() throw() : next(0), prev(0), data(0) {}
		inline ~node_type() throw() {}

		node_type *next, *prev;
		size_t     data;
       
        static inline int compare( const node_type *lhs, const node_type *rhs, void *)
        {
            return __compare(lhs->data,rhs->data);
        }
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
	};
	
	static inline void display( node_type *node ) throw()
	{
		std::cerr << node->data << std::endl;
	}
    
    static inline void display_list( const node_type *node, size_t n )
    {
        while(n-->0)
        {
            std::cerr << " " << node->data;
            node=node->next;
        }
        std::cerr << std::endl;
    }
    
}

YOCTO_UNIT_TEST_IMPL(list)
{
	const size_t num   = 30 + alea_leq(100);
	node_type   *nodes = new node_type[num];
	core::list_of<node_type> L;
	
    std::cerr << "-- inserting" << std::endl;
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
	std::cerr << "-- raw list" << std::endl;
    display_list(L.head,L.size);
    std::cerr << "-- sorting" << std::endl;
    core::merging<node_type>::sort(L,node_type::compare,NULL);
    display_list(L.head,L.size);
    
    
    std::cerr << "-- removing" << std::endl;
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
		if(curr)
            curr = 0;
            std::cerr << "unlink = " << node->data << std::endl;
        }
        
        for( size_t i=0; i < num; ++i )
        {
            L.push_back(nodes+i);
        }
        
        std::cerr << "-- raw list" << std::endl;
        display_list(L.root,L.size);
        std::cerr << "-- sorting" << std::endl;
        core::merging<node_type>::sort(L,node_type::compare,NULL);
        display_list(L.root,L.size);
        
        L.reset();
        delete [] nodes;
    }
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sequence/cached-list.hpp"


YOCTO_UNIT_TEST_IMPL(cached)
{
    typedef cache_of<node_type> node_cache;
    node_cache cache(12);
    cached_list<core::list_of,node_type>  L(cache);
    cached_list<core::clist_of,node_type> C(cache);
    
    for( size_t i=0; i < 100; ++i )
    {
        L.append()->data = i;
    }
    L.empty();
    
    for( size_t i=0; i < 100; ++i )
    {
        C.append()->data = i;
    }
    C.empty();
    std::cerr << "cache size=" << cache.size() << std::endl;
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/sequence/handle-list.hpp"


class h_node
{
public:
	inline h_node() throw() : prev(0), next(0), handle(0) {}
	inline ~h_node() throw() {}

    h_node   *prev;
    h_node   *next;
    unsigned *handle;
private:
	YOCTO_DISABLE_COPY_AND_ASSIGN(h_node);
};

YOCTO_UNIT_TEST_IMPL(hlist)
{
    unsigned arr[32];
    for( unsigned i=0; i < sizeof(arr)/sizeof(arr[0]); ++i ) arr[i] = i;
    cache_of<h_node> C;
    handle_list<h_node> L(C);
    
    for( unsigned i=0; i < sizeof(arr)/sizeof(arr[0]); ++i ) 
        L.attach( &arr[i] );
    
    
    
}
YOCTO_UNIT_TEST_DONE()

