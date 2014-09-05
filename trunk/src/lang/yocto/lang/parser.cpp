#include "yocto/lang/parser.hpp"
#include "yocto/lang/pattern/basic.hpp"

namespace yocto
{
    namespace lang
    {
        
        parser:: ~parser() throw()
        {
            
        }
        
        
        parser:: parser( const string &title, const string &mainScanner) :
        grammar(title+ " grammar"),
        lexer(  title+ " lexer"  ),
        scanner( declare(mainScanner) ),
        target( &scanner ),
        src()
        {
        }
        
        
        syntax::xnode * parser:: run( ios::istream &input )
        {
            return syntax::xnode::ast(accept(*this, src, input));
        }
        
        
#define YOCTO_LANG_PARSER_TERM_CODE(ARG,PPTY) \
target->make(label,ARG,forward); return term(label,syntax:: PPTY)
        
#define YOCTO_LANG_PARSER_TERM_IMPL(FUNCTION,PPTY) \
syntax::terminal & parser:: FUNCTION(const string &label,pattern *motif)\
{\
YOCTO_LANG_PARSER_TERM_CODE(motif,PPTY);\
}\
syntax::terminal & parser:: FUNCTION(const char *label,pattern *motif)\
{\
YOCTO_LANG_PARSER_TERM_CODE(motif,PPTY);\
}\
syntax::terminal & parser:: FUNCTION(const string &label, const string &regex)\
{\
YOCTO_LANG_PARSER_TERM_CODE(regex,PPTY);\
}\
syntax::terminal & parser:: FUNCTION(const char *label, const char *regex)\
{\
YOCTO_LANG_PARSER_TERM_CODE(regex,PPTY);\
}\
syntax::terminal & parser:: FUNCTION(const char *label, const char C)\
{\
target->make(label, single::create(C), forward);\
return term(label,syntax:: PPTY);\
}

        
        YOCTO_LANG_PARSER_TERM_IMPL(terminal,is_regular)
        YOCTO_LANG_PARSER_TERM_IMPL(jettison,is_discardable)
        YOCTO_LANG_PARSER_TERM_IMPL(univocal,is_specialized)
        


    }
}
