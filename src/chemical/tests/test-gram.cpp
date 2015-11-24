#include "yocto/lang/parser.hpp"
#include "yocto/lang/syntax/logic.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;


static const char level1_grammar[] =
{
#include "../yocto/chemical/level1.inc"
};

YOCTO_UNIT_TEST_IMPL(gram)
{
    auto_ptr<lang::parser> prs( lang::parser::generate("level1",level1_grammar,sizeof(level1_grammar)));
    
}
YOCTO_UNIT_TEST_DONE()
