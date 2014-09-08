#ifndef YOCTO_LANG_PARSER_INCLUDED
#define YOCTO_LANG_PARSER_INCLUDED 1§

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/grammar.hpp"

namespace yocto
{
    
    namespace lang
    {
        
#define YOCTO_LANG_PARSER_TERM_PROTO(FUNCTION) \
syntax::terminal & FUNCTION(const string &label,pattern      *motif);\
syntax::terminal & FUNCTION(const char   *label,pattern      *motif);\
syntax::terminal & FUNCTION(const string &label,const string &regex);\
syntax::terminal & FUNCTION(const char   *label,const char   *regex);\
syntax::terminal & FUNCTION(const char   *label,const char    C)
        
        //! high level
        class parser : public grammar, public lexer
        {
        public:
            typedef syntax::terminal  Terminal;
            typedef syntax::aggregate Aggregate;
            typedef syntax::alternate Alternate;
            
            explicit parser( const string &title, const string &mainScanner);
            virtual ~parser() throw();
            
            //! regular terminal(s)
            YOCTO_LANG_PARSER_TERM_PROTO(terminal);
            
            //! syntax only terminal(s)
            YOCTO_LANG_PARSER_TERM_PROTO(jettison);
            
            //! regular terminals withing meaningless content
            YOCTO_LANG_PARSER_TERM_PROTO(univocal);
            
            //! a named aggregate => syntax::is_regular
            syntax::aggregate &assemble(const string &label);
            
            //! wrapper
            syntax::aggregate &assemble(const char   *label);
            
            //! an all merging aggregate => syntax::is_merging_all
            syntax::aggregate &merge();
            
            
            //! a C-string detection to current target scanner
            syntax::rule &cstring(const string &label);
            
            
            syntax::xnode *run( ios::istream &input );
            
            //! create a new EOL comment for current target scanner
            void end_of_line_comment(const string &trigger);
            void end_of_line_comment(const char   *trigger);
            
        protected:
            lexical::scanner &scanner; //!< default scanner
            lexical::scanner *target;  //!< target scanner
            source             src;
            string             str;    //!< temporary workspace for string parsing if any
            
            void str_init(const token &) throw(); //!< initialize string
            void str_quit(const token &);         //!< finalize a new string
            bool str_emit(const token &);         //!< append token to str

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
            
        
        };
    }
}

#endif
