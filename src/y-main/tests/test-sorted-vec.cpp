#include "yocto/utest/run.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/sequence/vector.hpp"
#include "support.hpp"

#include <typeinfo>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(sorted_vec)
{
	
	{
		sorted_vector<size_t> sv;
	}
}
YOCTO_UNIT_TEST_DONE()
