#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/posix.hpp"
#include "yocto/utest/run.hpp"



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
    patterns.push_back( lang::zero_or_more( lang::single::create('z') ) );
    patterns.push_back( lang::one_or_more( lang::single::create('w') ) );
    patterns.push_back( lang::counting::create( lang::single::create('k'),1,3 ) );
    
    patterns.push_back( lang::posix::lower() );
    patterns.push_back( lang::posix::upper() );
    patterns.push_back( lang::posix::digit() );
    patterns.push_back( lang::posix::alpha() );
    patterns.push_back( lang::posix::alnum() );
    patterns.push_back( lang::posix::xdigit() );
    patterns.push_back( lang::posix::blank() );
    patterns.push_back( lang::posix::space() );
    patterns.push_back( lang::posix::punct() );
   
    patterns.push_back( lang::posix::word() );
    patterns.push_back( lang::posix::endl() );
    patterns.push_back( lang::posix::dot() );
    patterns.push_back( lang::posix::cstring() );

    
    {
        lang::logical *p = lang::AND::create();
        patterns.push_back(p);
        p->operands.push_back( lang::any1::create()  );
        
    }
}
YOCTO_UNIT_TEST_DONE()
