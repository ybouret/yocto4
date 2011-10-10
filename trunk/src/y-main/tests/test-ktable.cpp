#include "yocto/utest/run.hpp"
#include "yocto/associative/ktable.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace 
{
	class dummy
	{
	public:
		dummy       *next;
		dummy       *prev;
		const size_t hkey;
		
		dummy( size_t k ) : next(NULL), prev(NULL), hkey(k) 
		{
			//std::cerr << "+dummy[" << a << "]" << std::endl;
			//(std::cerr << "+" ).flush();
		}
		~dummy() throw() 
		{
			//std::cerr << "-dummy[" << a << "]" << std::endl;
			//(std::cerr << "-" ).flush();
		}
		
		static void kill( dummy *node ) throw()
		{
			destruct(node);
		}
		
		static inline bool match( const dummy *dn, const void *param ) throw()
		{
			return dn->hkey == *(size_t *)param;
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(dummy);
	};
}

YOCTO_UNIT_TEST_IMPL(ktable)
{
	
	memory::global::allocator alloc;
	
	{ core::ktable<dummy> tab; }
	
	for( size_t i=0; i <= 3000; i += (1+alea_less_than(30)) )
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
		
		const size_t kmax = 2*i;
		for( size_t j=0; j < tab.nodes; ++j )
		{
			dummy *dn = new(tab.cache.query()) dummy( alea_less_than(kmax) );
			tab.insert(dn);
		}
		
		const size_t nmax = 64;
		size_t       nout = 0;
		std::cerr << "search[";
		for( size_t j=0; j < tab.nodes; ++j )
		{
			core::ktable<dummy>::kslot_t *slot = NULL;
			const size_t k  = alea_less_than(kmax);
			const dummy *dn = tab.search(k, slot, dummy::match, &k);
			if( dn )
			{
				(std::cerr << ".").flush();
				tab.remove_front_of(slot, dummy::kill );
				if( ++nout >= nmax ) break;
			}
		}
		std::cerr << "]" << std::endl;
		
		
		
		
		tab.free_with( dummy::kill );		
		tab.release_all( alloc );
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
