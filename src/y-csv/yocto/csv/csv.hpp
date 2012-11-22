#ifndef YOCTO_CSV_INCLUDED
#define YOCTO_CSV_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    
    namespace CSV
    {
        typedef vector<string> Line;
        typedef vector<Line>   Lines;
        
    }
    
}

#endif
