#ifndef YOCTO_COMBINATION_INCLUDED
#define YOCTO_COMBINATION_INCLUDED 1

#include "yocto/os.hpp"
#include <iosfwd>

namespace yocto
{
    
    class combination
    {
    public:
        static void init( size_t comb[], const size_t k, const size_t n ) throw();
        static bool next( size_t comb[], const size_t k, const size_t n ) throw();
        
    private:
        YOCTO_DISABLE_ASSIGN(combination);
       
    };
    
}

#endif
