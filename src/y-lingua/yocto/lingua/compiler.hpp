#ifndef YOCTO_LINGUA_COMPILER_INCLUDED
#define YOCTO_LINGUA_COMPILER_INCLUDED 1

#include "yocto/lingua/parser.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        //! compile a grammar into an AST
        class compiler : public parser
        {
        public:
            explicit compiler();
            virtual ~compiler() throw();
            
            static
            syntax::xnode *ast( syntax::xnode *node);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(compiler);
            void do_nothing(const token &) throw();
            void do_newline(const token &) throw();
            
        };
        
    }
    
}

#endif
