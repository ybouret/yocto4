#ifndef YOCTO_LINGUA_COMPILER_INCLUDED
#define YOCTO_LINGUA_COMPILER_INCLUDED 1

#include "yocto/lingua/parser.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        class compiler : public parser
        {
        public:
            explicit compiler();
            virtual ~compiler() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(compiler);
        };
        
    }
    
}

#endif
