#include "yocto/chemical/collection.hpp"
#include "yocto/utest/run.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(collection)
{
    chemical::collection lib;
    lib.reserve(16);
    
    chemical::species &proton   = lib.add("H+",1);
    chemical::species &hydroxy  = lib.add("HO-",-1);
    chemical::species &chloride = lib.add("Cl-");
    
    proton.data.allocate(100);
    hydroxy.data.make<double>(3.0);
    chloride.data.make<int>();
    
    for( chemical::collection::iterator i = lib.begin(); i != lib.end(); ++i )
    {
        const chemical::species &sp = **i;
        std::cerr << sp << " +#bytes=" << sp.data.size() << std::endl;
    }
    
    std::cerr << "ztable: " << std::endl;
    lib.dump_ztable(std::cerr);
    
    
}
YOCTO_UNIT_TEST_DONE()

