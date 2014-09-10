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
            
            virtual syntax::xnode *run( source &src );
            
            void emit( const syntax::xnode *G );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(compiler);
            void do_nothing(const token &) throw();
            void do_newline(const token &) throw();
            static syntax::xnode * rewrite( syntax::xnode *) throw();
            static syntax::xnode * contract_atoms( syntax::xnode *node ) throw();
            static syntax::xnode * assemble( const char *label, syntax::xnode *node ) throw();
            static syntax::xnode * simplify( syntax::xnode *node ) throw();
            
        };
        
    }
    
}

#endif
