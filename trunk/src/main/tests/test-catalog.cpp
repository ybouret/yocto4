#include "yocto/utest/run.hpp"
#include "yocto/ordered/catalog.hpp"
#include "yocto/sequence/vector.hpp"
#include "support.hpp"

#include <typeinfo>

using namespace yocto;


template <typename T,typename ALLOCATOR>
void test_catalog()
{
	YOCTO_ARGUMENTS_DECL_T;

	typedef catalog<T,comparator<mutable_type>,ALLOCATOR> cat_t;
	std::cerr << "catalog<" << typeid(T).name() << ">: size=" << sizeof(cat_t) << std::endl;
	
	cat_t C;
	vector<const_type> objs;
	
	for( size_t i=0; i < 4096; ++i )
	{
		type tmp = gen<mutable_type>::get();
		if( C.insert( (param_type)tmp ) )
		{
			if( alea<float>() > 0.6f ) 
			{
				objs.push_back(tmp);
			}
		}
	}
	
	cat_t C2( C );
	for( size_t i=1; i <= objs.size(); ++i )
	{
		if( ! C.search(objs[i]) )
			std::cerr << "search error level-1" << std::endl;
		
		if( ! C2.search( objs[i] ) )
			std::cerr << "search error level-2" << std::endl;
		
		if( !C2.remove( objs[i] ) )
			std::cerr << "remove error level-3" << std::endl;
	}
	
	for( typename cat_t::const_iterator i = C2.begin(); i != C2.end() ; ++i )
	{
		if( ! C.remove( *i ) )
		{
			std::cerr << "remove error level-4" << std::endl;
		}
	}
	
	
}

YOCTO_UNIT_TEST_IMPL(catalog)
{

	test_catalog<int,memory::global::allocator>();
	test_catalog<string,memory::global::allocator>();
	
}
YOCTO_UNIT_TEST_DONE()
