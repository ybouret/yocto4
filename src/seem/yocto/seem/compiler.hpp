#ifndef YOCTO_SEEM_COMPILER_INCLUDED 
#define YOCTO_SEEM_COMPILER_INCLUDED 1

#include "yocto/seem/types.hpp"

namespace yocto
{
    namespace Seem
    {
        
        class Compiler
        {
        public:
            virtual ~Compiler() throw();
            explicit Compiler(const bool emitFiles);

            Parser     parser;
            TreeWalker walker;

            void compile( ios::istream &fp );
            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Compiler);

        };

    }
}

#endif
