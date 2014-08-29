#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(pattern)
{
    std::cerr << "sizeof(lang::pattern)=" << sizeof(lang::pattern) << std::endl;
    lang::p_list patterns;
    patterns.push_back( lang::any1::create()         );
    patterns.push_back( lang::single::create('A')    );
    patterns.push_back( lang::range::create('A','Z') );
    patterns.push_back( lang::within::create()       );
    patterns.push_back( lang::none::create()         );
    patterns.push_back( lang::optional::create( lang::single::create('B') ) );
    patterns.push_back( lang::at_least::create( lang::single::create('q'),0) );
                       
}
YOCTO_UNIT_TEST_DONE()
