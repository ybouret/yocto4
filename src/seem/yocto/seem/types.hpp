#ifndef YOCTO_SEEM_TYPES_INCLUDED
#define YOCTO_SEEM_TYPES_INCLUDED 1

#include "yocto/lingua/parser.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace Seem
    {
        typedef lingua::syntax::xnode       XNode;
        typedef uint32_t                    OpCode;
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

