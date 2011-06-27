#include "yocto/utest/run.hpp"
#include "yocto/core/xarray.hpp"
#include "yocto/memory/global.hpp"

#include "support.hpp"
#include <typeinfo>

using namespace yocto;

template <typename T>
static inline void test_xarray()
{
	std::cerr << "xarray<" << typeid(T).name() << ">" << std::endl;
	{ core::xarray<T> xarr; }
	memory::global::allocator alloc;
	
	for( size_t n=0; n <= 32; ++n )
	{
		core::xarray<T> arr(n,alloc);
		std::cerr << "#" << n << " => " << arr.bytes() << " bytes" << std::endl;
		
		size_t count = 0;
		for( size_t i=0; i <n; ++i )
		{
			T  tmp = gen<T>::get();
			T *p   = arr.slots.query();
			try {
				new (p) T(tmp);
				arr.table[count] = p;
				++count;
			}
			catch(...)
			{
				arr.slots.store(p);
			}
		}
		
		c_shuffle(arr.table, count);
		for( size_t i=0; i < count; ++i )
		{
			T *p = arr.table[i];
			destruct(p);
			arr.slots.store(p);
		}
		
		
		arr.release_all(alloc);
	}
	
}

YOCTO_UNIT_TEST_IMPL(xarray)
{

	test_xarray<int>();
	test_xarray<double>();
	test_xarray<string>();
}
YOCTO_UNIT_TEST_DONE()
