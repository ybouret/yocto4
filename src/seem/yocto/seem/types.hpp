#ifndef YOCTO_SEEM_TYPES_INCLUDED
#define YOCTO_SEEM_TYPES_INCLUDED 1

#include "yocto/lingua/parser.hpp"

namespace yocto
{
    namespace Seem
    {
        typedef lingua::parser Parser;


        Parser *CreateParser(const bool emitFiles=false);
        
    }
}

#endif

