#include "yocto/utest/run.hpp"
#include "yocto/python/python.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(vm)
{
    python &Python = python::instance();
    
    std::cerr << "using " << Python.name << std::endl;
    
    Python.Initialize();
    
    Python.Finalize();
    
}
YOCTO_UNIT_TEST_DONE()
