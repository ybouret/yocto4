#ifndef YOCTO_LANG_COMPILER_INCLUDED
#define YOCTO_LANG_COMPILER_INCLUDED 1


#include "yocto/lang/parser.hpp"


namespace yocto 
{
    
    namespace lang 
    {
        
        //! compile a simple grammar from an input stream
        /**
         parse a compact EBNF grammar
         into an AST.
         */
        class compiler : public parser
        {
        public:
            explicit  compiler();
            virtual  ~compiler() throw();
            
            syntax::parse_node * operator()( ios::istream &fp );
            grammar            * build( syntax::parse_node *ast) const;
            
            
        private:
            lexical::scanner &comment;
            void              enter_comment( const regex::token &) throw();
            void              leave_comment( const regex::token &) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(compiler);
            
        };
        
    }
    
}

#endif

