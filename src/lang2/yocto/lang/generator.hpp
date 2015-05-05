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
            virtual ~generator() throw();
            
            explicit generator(const char *langID);
            
            
            syntax::xnode *compile( ios::istream &fp );
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(generator);
            void reshape(syntax::xnode *node) throw();
            void rewrite(syntax::xnode *node);
        };
        
    }
}

#endif
