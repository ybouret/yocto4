#include "yocto/utest/run.hpp"
#include "yocto/seem/compiler.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/graphviz.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(compiler)
{
    Seem::Compiler cmp(true);
    
    ios::icstream  fp( ios::cstdin );
    string         code;
    cmp.run(code,fp);


    ios::wcstream  bin( "bin.dat" );
    bin << code;
}
YOCTO_UNIT_TEST_DONE()

