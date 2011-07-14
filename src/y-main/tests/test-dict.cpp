#include "yocto/utest/run.hpp"
#include "yocto/associative/dictionary.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/memory/pooled.hpp"

#include "support.hpp"
#include <typeinfo>

using namespace yocto;


template <typename KEY,typename T,typename COMPARATOR, typename ALLOCATOR>
static inline void test_dict()
{
	typedef  dictionary<KEY,T,COMPARATOR,ALLOCATOR> dict_t;
	
	std::cerr << "dictionary<" << typeid(KEY).name() << "," << typeid(T).name() << ">" << std::endl;
	std::cerr << "sizeof(dict)=" << sizeof(dict_t) << std::endl;
	{ dict_t M; }
	{ dict_t M(1,as_capacity); }
	
	{
		std::cerr << "---- generating" << std::endl;
		dict_t M;
		vector<KEY> keys;
		for( size_t i=0; i < 2048; ++i )
		{
			KEY key  = gen< typename dict_t::mutable_key  >::get();
			T   data = gen< typename dict_t::mutable_type >::get();
			if( ! M.insert( key, data ) )
			{
				std::cerr << "Multiple " << key << std::endl;
			}
			else
			{
				if( alea<float>() > 0.5f )
				{
					keys.push_back(key);
				}
			}
		}
		std::cerr << "---- size=" << M.size() << "/" << M.capacity() << std::endl;
		std::cerr << "---- copy" << std::endl;
		dict_t M2(M);
		
		std::cerr << "---- removing " << keys.size() << " keys" << std::endl;
		for( size_t i=1; i <= keys.size(); ++i )
		{
			if( ! M.search( keys[i] ) )
			{
				std::cerr << "Unable to find '" << keys[i] << "' !" << std::endl;

			}
			
			if( ! M2.remove( keys[i] ) )
			{
				std::cerr << "Unable to remove '" << keys[i] << "' !" << std::endl;
			}
		}
		
		const dict_t &D = M2;
		for( typename dict_t::const_iterator i= D.begin(); i != D.end(); ++i )
		{
			(void) *i;
		}

		for( typename dict_t::iterator i = M2.begin(); i != M2.end(); ++i )
		{
			(void) *i;
			if( ! M.remove( i->key ) )
			{
				std::cerr << "Unable to remove '" << i->key << "' !" << std::endl;
			}

		}
	}
	
}

YOCTO_UNIT_TEST_IMPL(dict)
{
	test_dict<int,int,comparator<int>,memory::global::allocator>();
	test_dict<int,string,comparator<int>,memory::global::allocator>();
	test_dict<string,string,comparator<string>,memory::pooled::allocator>();
}
YOCTO_UNIT_TEST_DONE()
