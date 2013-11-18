#include "yocto/threading/at-exit.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

static void func1() throw()
{
	std::cerr << "func1" << std::endl;
}

static void func2() throw()
{
	std::cerr << "func2" << std::endl;
}

static void func3() throw()
{
	std::cerr << "func3" << std::endl;
}



YOCTO_UNIT_TEST_IMPL(at_exit)
{
	threading::at_exit::perform( func1, 10 );
	threading::at_exit::perform( func2, 12 );
	threading::at_exit::perform( func3, 2  );
}
YOCTO_UNIT_TEST_DONE()
