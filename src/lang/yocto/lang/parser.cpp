#include "yocto/lang/parser.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/posix.hpp"

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

        
        YOCTO_LANG_PARSER_TERM_IMPL(terminal,is_standard)
        YOCTO_LANG_PARSER_TERM_IMPL(jettison,is_jettison)
        YOCTO_LANG_PARSER_TERM_IMPL(univocal,is_univocal)
        
        syntax::aggregate & parser::assemble(const string &label)
        {
            return agg(label,syntax::is_standard);
        }
        
        syntax::aggregate & parser::assemble(const char   *label)
        {
            const string Label(label);
            return assemble(Label);
        }

        syntax::aggregate & parser:: merge()
        {
            const string label = vformat("@blend/%d",++counter);
            return agg(label,syntax::is_merging_all);
        }

        
        void parser:: end_of_line_comment(const string &trigger)
        {
            const string com = "@Comment:" + trigger;
			if(!has(com))
			{
				//create the new scanner
				lexical::scanner &sub = declare(com);
				sub.make("CHAR", posix::dot(), discard);
				sub.back(posix::endl(),endl_cb);
			}

			// call the comment upon trigger
			target->call(com,trigger,nothing);
        }
        
        void parser:: end_of_line_comment(const char   *trigger)
        {
            const string Trigger(trigger);
            end_of_line_comment(Trigger);
        }

        
    }
}
