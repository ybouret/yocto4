#include "yocto/utest/run.hpp"
#include "yocto/chemical/boot.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(boot)
{
    std::cerr << "sizeof(boot:constituent)=" << sizeof(chemical::boot::constituent) << std::endl;
    std::cerr << "sizeof(boot:constraint)="  << sizeof(chemical::boot::constraint) << std::endl;
    std::cerr << "sizeof(boot:loader)="       << sizeof(chemical::boot::loader) << std::endl;

    chemical::boot::loader ini;
    
    
}
YOCTO_UNIT_TEST_DONE()
