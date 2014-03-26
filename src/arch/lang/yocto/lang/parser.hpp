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
            explicit parser( const char   *lexer_main_id, const char   *grammar_name );
            virtual ~parser() throw();
            
            //! lexer main scanner
            /**
             its dictionary is used for regex compilation
             */
            lexical::scanner &scan; 
            
            //! make a terminal for lexer and for grammar
            syntax::terminal &terminal( const string &id, const string &expr,   syntax::node_property ppty = syntax::is_regular );
            syntax::terminal &terminal( const string &id, const char    single, syntax::node_property ppty = syntax::is_regular );
            syntax::terminal &terminal( const char   *id, const char   *expr,   syntax::node_property ppty = syntax::is_regular );
            syntax::terminal &terminal( const char   *id, const char    single, syntax::node_property ppty = syntax::is_regular );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
        
    }
}

#endif
