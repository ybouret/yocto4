#include "yocto/utest/run.hpp"
#include "yocto/container/xarray.hpp"
#include "yocto/memory/global.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(xarray)
{
	{ core::xarray<int> ixarr; }
	memory::global::allocator alloc;
	core::xarray<int> arr(1,alloc);
	
}
YOCTO_UNIT_TEST_DONE()
