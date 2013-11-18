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
            
            
            //------------------------------------------------------------------
            //
            // generic terminal interface
            //
            //------------------------------------------------------------------
            
            //! create a lexer rule and a REGULAR grammar terminal
            syntax::terminal & terminal( const string &id, const string &expr, syntax::node_property ppty = syntax::is_regular);
            
            //! create a lexer rule and a REGULAR grammar terminal
            syntax::terminal & terminal( const char *id, const char *expr, syntax::node_property ppty = syntax::is_regular);
            
            //! create a lexer rule and a SINGLE CHAR grammar terminal
            syntax::terminal & terminal( const string &id, const char C, syntax::node_property ppty = syntax::is_regular);
            
            //! create a lexer rule and a SINGLE CHAR grammar terminal
            syntax::terminal & terminal( const char *id, const char C, syntax::node_property ppty = syntax::is_regular);
            
            //------------------------------------------------------------------
            //
            // specialized terminal: node is kept, content is dropped
            //
            //------------------------------------------------------------------
            //! create a lexer rule and a SPECIALIZED grammar terminal
            syntax::terminal & univocal( const string &id, const string &expr );
            
            //! create a lexer rule and a SPECIALIZED grammar terminal
            syntax::terminal & univocal( const char   *id, const char   *expr );
            
            //! a specialized single char
            syntax::terminal & univocal( const string &id, const char C );
            
            //! a specialized single char
            syntax::terminal & univocal( const char *id, const char C );
            
            //------------------------------------------------------------------
            //
            // discardable terminal: node is dropped (no semantic meaning)
            //
            //------------------------------------------------------------------
            //! create a lexer rule and a DISCARDABLE grammar terminal (no semantic role)
            syntax::terminal & jettison( const string &id, const string &expr);
            
            //! create a lexer rule and a DISCARDABLE grammar terminal (no semantic role)
            syntax::terminal & jettison( const char *id, const char *expr );
            
            //! jettison a single char
            syntax::terminal & jettison( const string &id, const char C );
            
            //! jettison a single char
            syntax::terminal & jettison( const char *id, const char C );
            
            
            //! accept from source and make AST
            syntax::xnode *run( source &src );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
        
        
    }
}

#endif
