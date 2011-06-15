#include "yocto/utest/run.hpp"
#include "yocto/string/env.hpp"

using namespace yocto;

static inline
void display( const string &name, const string &value, void * )
{
	std::cerr << name << " => " << value << std::endl;
}

YOCTO_UNIT_TEST_IMPL(env)
{
	//-- display all
	environment::get( display, NULL );
}
YOCTO_UNIT_TEST_DONE()
