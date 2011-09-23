#include "yocto/utest/run.hpp"
#include "yocto/rx/parser.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(parser)
{

	std::cerr << "sizeof(syntactic::rule)=" << sizeof(regex::syntactic::rule) << std::endl;

}
YOCTO_UNIT_TEST_DONE()
