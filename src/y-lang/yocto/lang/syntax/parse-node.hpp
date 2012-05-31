#ifndef YOCTO_LANG_SYNTAX_PARSE_NODE_INCLUDED
#define YOCTO_LANG_SYNTAX_PARSE_NODE_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/code/round.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            
            class parse_node : public object
            {
            public:
                typedef core::list_of<parse_node> child_list;
                const string &label;    //!< reference to the corresponding rule label
                const bool    terminal; //!< true is terminal => one lexeme
                parse_node   *prev;
                parse_node   *next;
                parse_node   *parent;
                
                virtual ~parse_node() throw();
                explicit parse_node( const string &label_ref, lexeme *lx ) throw();
                
                lexeme     * lex() throw();
                child_list & children() throw();
                
            private:
                uint64_t wksp[ YOCTO_U64_FOR_ITEM(child_list) ];
                YOCTO_DISABLE_COPY_AND_ASSIGN(parse_node);
            };
        }
    }
    
}


#endif
