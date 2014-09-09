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
            typedef syntax::rule      Rule;
            
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
            
            //! wrapper with 2 rules
            syntax::aggregate &assemble(const char *label, Rule &r1, Rule &r2 );
            
            //! wrapper with 3 rules
            syntax::aggregate &assemble(const char *label, Rule &r1, Rule &r2, Rule &r3 );

            
            
            //! an all merging aggregate => syntax::is_merging_all
            syntax::aggregate &merge();
            
            //! wrapper with 2 rules
            syntax::aggregate &merge(Rule &r1, Rule &r2);

            //! wrapper with 3 rules
            syntax::aggregate &merge(Rule &r1, Rule &r2, Rule &r3);
            
            
            //! wrapper with 2 rules
            syntax::alternate &choose(Rule &r1,Rule &r2);
            
            //! wrapper with 3 rules
            syntax::alternate &choose(Rule &r1,Rule &r2, Rule &r3);

            //! main code
            syntax::xnode *run( ios::istream &input );
            
            //! plugin: current target call the plugin upon its trigger
            /**
             assume that the plugin ungets a TERMINAL lexeme (e.g.: string)
             */
            syntax::rule &plug_term( lexical::plugin *plugin );
            
            //! meta data plugin, no grammatical sense (e.g. comment)
            void          plug_meta( lexical::plugin *plugin );
            
            
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
