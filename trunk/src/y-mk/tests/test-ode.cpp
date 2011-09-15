#include "yocto/utest/run.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include <typeinfo>


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(lagrange)
{

	vector<double> x;
	vector<double> y;
	
	const size_t n = 2 + alea_less_than( 10 );
	
	
	
}
YOCTO_UNIT_TEST_DONE()
