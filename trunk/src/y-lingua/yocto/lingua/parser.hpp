#ifndef YOCTO_LINGUA_PARSER_INCLUDED
#define YOCTO_LINGUA_PARSER_INCLUDED 1

#include "yocto/lingua/grammar.hpp"

namespace yocto
{
    namespace lingua
    {
        
        class parser : public lexer, public grammar
        {
        public:
            explicit parser( const string &title, const string &mainID );
            virtual ~parser() throw();
            
            lexical::scanner         &scanner; //!< first, used to compile regex
            
            //! create a lexer rule and a REGULAR grammar terminal
            syntax::terminal & terminal( const string &id, const string &expr, syntax::node_property ppty = syntax::is_regular);
        
            //! create a lexer rule and a REGULAR grammar terminal
            syntax::terminal & terminal( const char *id, const char *expr, syntax::node_property ppty = syntax::is_regular);
        
            //! crate a lexer rule and a SPECIALIZED grammar terminal
            syntax::terminal & univocal( const string &id, const string &expr );
            
            //! crate a lexer rule and a SPECIALIZED grammar terminal(drop content)
            syntax::terminal & univocal( const char   *id, const char   *expr );
            
            //! create a lexer rule and a DISCARDABLE grammar terminal (no semantic role)
            syntax::terminal & jettison( const string &id, const string &expr);
            
            //! create a lexer rule and a DISCARDABLE grammar terminal (no semantic role)
            syntax::terminal & jettison( const char *id, const char *expr );
            
            virtual syntax::xnode *run( source &src );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
        
        
    }
}

#endif
