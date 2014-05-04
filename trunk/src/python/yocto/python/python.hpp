#ifndef YOCTO_PYTHON_INCLUDED
#define YOCTO_PYTHON_INCLUDED

#include <Python.h>
#include "yocto/threading/singleton.hpp"

namespace yocto
{
 
    class python : public singleton<python>
    {
    public:
        
        static const char name[];
        void Initialize();
        void Finalize() throw();
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(python);
        friend class singleton<python>;
        explicit python() throw();
        virtual ~python() throw();
        static const threading::longevity life_time = 10;
    };
    
}

#endif
