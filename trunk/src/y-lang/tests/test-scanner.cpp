#include "yocto/utest/run.hpp"
#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/rx/pattern/posix.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(scanner)
{

    lang::lexical::scanner scan("dummy");
    
    scan.dict().record( "DIGIT", regex::posix::digit() );
    
    scan( "INT", "{DIGIT}+" );
    
}
YOCTO_UNIT_TEST_DONE()
