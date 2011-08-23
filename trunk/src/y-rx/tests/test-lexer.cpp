#include "yocto/utest/run.hpp"
#include "yocto/rx/lexer.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(lexer)
{
	std::cerr << "sizeof(regex::lexical::engine)= " << sizeof(regex::lexical::engine) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
