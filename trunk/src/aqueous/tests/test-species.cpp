#include "yocto/utest/run.hpp"
#include "yocto/aqueous/species.hpp"

using namespace yocto;
using namespace aqueous;

YOCTO_UNIT_TEST_IMPL(species)
{
    species h("H+",1,100);
    h.make<double,double>(0);
    h.make<uint64_t,int>(19);
    h.make<float,float>(3.0f);
    std::cerr << h.get<float>() << std::endl;
    
    species w("HO-",-1,sizeof(string));
    w.make<string,string>( "hello" );
    std::cerr << w.get<string>() << std::endl;
}
YOCTO_UNIT_TEST_DONE()
