#ifndef YOCTO_LANG_PARSER_INCLUDED
#define YOCTO_LANG_PARSER_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/grammar.hpp"

namespace yocto 
{
    
    namespace lang 
    {
        
        //! wrapper for parser creation
        class parser : public lexer, public grammar 
        {
        public:
            explicit parser( const string &lexer_main_id, const string &gammar_name );
            virtual ~parser() throw();
            
            //! lexer main scanner
            /**
             its dictionary is used for regex compilation
             */
            lexical::scanner &scan; 
            
            //! make a terminal for lexer and for grammar
            syntax::terminal &terminal( const string &id, const string &expr, syntax::node_property ppty = syntax::is_regular );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
        
    }
}

#endif
