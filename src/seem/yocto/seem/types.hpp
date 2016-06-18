#ifndef YOCTO_SEEM_TYPES_INCLUDED
#define YOCTO_SEEM_TYPES_INCLUDED 1

#include "yocto/lingua/parser.hpp"
#include "yocto/lingua/syntax/tree-walker.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace Seem
    {
        typedef lingua::syntax::xnode       XNode;
        typedef lingua::syntax::tree_walker TreeWalker;

        //lingua::parser *CreateSeemParser(const bool emitFiles=false);

        class Parser
        {
        public:
            explicit Parser(const bool emitFile=false);
            virtual ~Parser() throw();

            arc_ptr<lingua::parser>          impl;
            arc_ptr<lingua::syntax::grammar> gram;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }
}

#endif

