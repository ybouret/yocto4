#ifndef YOCTO_SEEM_TYPES_INCLUDED
#define YOCTO_SEEM_TYPES_INCLUDED 1

#include "yocto/lingua/parser.hpp"
#include "yocto/lingua/syntax/tree-walker.hpp"

namespace yocto
{
    namespace Seem
    {
        typedef lingua::parser              Parser;
        typedef lingua::syntax::xnode       XNode;
        typedef lingua::syntax::tree_walker TreeWalker;

        Parser *CreateParser(const bool emitFiles=false);
        
    }
}

#endif

