#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/dict.hpp"
#include "yocto/lingua/pattern/posix.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lingua;


YOCTO_UNIT_TEST_IMPL(dict)
{
    std::cerr << "sizeof(p_dict)=" << sizeof(p_dict) << std::endl;
    
    p_dict D;
    
    D.insert( "alpha", posix::alpha() );
    D.insert( "alnum", posix::alnum() );
    D.insert( "digit", posix::digit() );
    
    auto_ptr<pattern> q( D.create( "digit" ) );
    
}
YOCTO_UNIT_TEST_DONE()

