#include "yocto/utest/run.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/hashing/sfh.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/memory/pooled.hpp"

#include "yocto/sequence/vector.hpp"

#include "support.hpp"
#include <typeinfo>

using namespace yocto;

template <typename KEY,typename T,typename KEY_HASHER, typename ALLOCATOR>
static inline void test_map()
{
	typedef  map<KEY,T,KEY_HASHER,ALLOCATOR> map_t;
	std::cerr << "map<" << typeid(KEY).name() << "," << typeid(T).name() << ">" << std::endl;
	std::cerr << "sizeof(map)=" << sizeof(map_t) << std::endl;
	{ map_t M; }
	{ map_t M(1,as_capacity); }
	
	{
		std::cerr << "---- generating" << std::endl;
		map_t M;
		vector<KEY> keys;
		for( size_t i=0; i < 2048; ++i )
		{
			KEY key  = gen< typename map_t::mutable_key  >::get();
			T   data = gen< typename map_t::mutable_type >::get();
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
		map_t M2(M);
		for( size_t i=1; i <= keys.size(); ++i )
		{
			if( ! M2.remove( keys[i] ) )
			{
				std::cerr << "Unable to remove '" << keys[i] << "' !" << std::endl;
			}
		}
		
		for( typename map_t::iterator i = M2.begin(); i != M2.end(); ++i )
		{
			if( !M.remove( i->key ) )
			{
				std::cerr << "Unable to remove '" << i->key << "' !" << std::endl;
			}
		}
		
		
		
	}
}

YOCTO_UNIT_TEST_IMPL(map)
{
	test_map<int,int,key_hasher<int>,memory::global::allocator>();
	test_map<const int,string,key_hasher<int,hashing::sfh>,memory::global::allocator>();
	test_map<const string,const string,key_hasher<string,hashing::sha1>,memory::pooled::allocator>();
}
YOCTO_UNIT_TEST_DONE()
