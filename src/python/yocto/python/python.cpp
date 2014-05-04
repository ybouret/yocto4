#include "yocto/python/python.hpp"

namespace yocto
{
    
    python:: python() throw(){}
    
    python:: ~python() throw() {}
                            
    const char python::name[] = "python";
    
    void python::Initialize()
    {
        Py_Initialize();
    }
    
    
    void python:: Finalize() throw()
    {
        Py_Finalize();
    }
}
