#ifndef YOCTO_SEEM_COMPILER_INCLUDED 
#define YOCTO_SEEM_COMPILER_INCLUDED 1

#include "yocto/seem/types.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace Seem
    {

        class Compiler : public TreeWalker
        {
        public:
            virtual ~Compiler() throw();
            explicit Compiler();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Compiler);
            arc_ptr<Parser> parser;
        };

    }
}

#endif
