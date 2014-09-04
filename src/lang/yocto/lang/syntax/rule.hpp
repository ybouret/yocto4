#ifndef YOCTO_LANG_SYNTAX_RULE_INCLUDED
#define YOCTO_LANG_SYNTAX_RULE_INCLUDED 1

#include "yocto/lang/syntax/xnode.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            
#define YOCTO_LANG_SYNTAX_RULE_ARGS \
lexer &Lexer, source &Source, ios::istream &Input, xtree &Tree
            
#define YOCTO_LANG_SYNTAX_RULE_MATCH_DECL() \
virtual bool match(YOCTO_LANG_SYNTAX_RULE_ARGS)
            
#define YOCTO_LANG_SYNTAX_RULE_MATCH_IMPL(CLASS) \
bool CLASS:: match(YOCTO_LANG_SYNTAX_RULE_ARGS)
            
            class rule : public object
            {
            public:
                const string label;
                rule        *next;
                rule        *prev;
                
                virtual ~rule() throw();
                
                //! set tree to node or append node to tree
                static void grow( xtree &Tree, xtree &Node) throw();
                
                //! matching interface
                /**
                 if doesn't match, must restore what was get from Lexer
                 */
                YOCTO_LANG_SYNTAX_RULE_MATCH_DECL() = 0;
                
            protected:
                explicit rule(const string &id );
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            };
            
            typedef core::list_of_cpp<rule> r_list;
            
        }
    }
}

#endif

