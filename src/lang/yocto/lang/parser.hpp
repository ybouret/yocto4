#ifndef YOCTO_LANG_PARSER_INCLUDED
#define YOCTO_LANG_PARSER_INCLUDED 1§

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/grammar.hpp"

namespace yocto
{
    
    namespace lang
    {
        
#define YOCTO_LANG_PARSER_TERM_PROTO(FUNCTION) \
syntax::terminal & FUNCTION(const string &label,pattern *motif);\
syntax::terminal & FUNCTION(const char   *label,pattern *motif);\
syntax::terminal & FUNCTION(const string &label,const string &regex);\
syntax::terminal & FUNCTION(const char   *label,const char   *regex);\
syntax::terminal & FUNCTION(const char   *label,const char   C)
        
        //! high level
        class parser : public grammar, public lexer
        {
        public:
            typedef syntax::terminal  Terminal;
            typedef syntax::aggregate Aggregate;
            typedef syntax::alternate Alternate;
            
            explicit parser( const string &title, const string &mainScanner);
            virtual ~parser() throw();
            
            YOCTO_LANG_PARSER_TERM_PROTO(terminal);
            YOCTO_LANG_PARSER_TERM_PROTO(jettison);
            YOCTO_LANG_PARSER_TERM_PROTO(univocal);
            
            syntax::xnode *run( ios::istream &input );
            
        protected:
            lexical::scanner &scanner; //!< default scanner
            lexical::scanner *target;  //!< target scanner
            source             src;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
        };
    }
}

#endif
