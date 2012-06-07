#ifndef YOCTO_LANG_PARSER_INCLUDED
#define YOCTO_LANG_PARSER_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/lang/grammar.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        class parser
        {
        public:
            explicit parser( const string &name);
            virtual ~parser() throw();
            
            regex::pattern_dict &dict();
            void                 no_dict() throw();
            
            void terminal( const string &id, const string &expr, syntax::node_property = syntax::is_regular );
            
        protected:
            lexer             L;    //!< the lexer
            grammar           G;    //!< associated grammar
            lexical::scanner &scan; //!< main scanner
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
            
        };
        
    }
    
}

#endif
