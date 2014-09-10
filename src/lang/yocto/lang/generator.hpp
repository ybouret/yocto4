#ifndef YOCTO_LANG_GENERATOR_INCLUDED
#define YOCTO_LANG_GENERATOR_INCLUDED 1

#include "yocto/lang/parser.hpp"


namespace yocto
{
    namespace lang
    {
        
        class generator : public parser
        {
        public:
            explicit generator();
            virtual ~generator() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(generator);
        };
        
    }
}

#endif

