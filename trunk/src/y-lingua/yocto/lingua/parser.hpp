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
            
            lexical::scanner &scanner; //!< first, used to compile regex
            
            //! create a lexer rule and a grammar terminal
            syntax::terminal & terminal( const string &id, const string &expr);
            
            //! create a lexer rule and a grammar terminal
            syntax::terminal & terminal( const char *id, const char *expr);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
        
        
    }
}

#endif
