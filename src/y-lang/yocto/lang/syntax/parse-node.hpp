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
                const bool    terminal; //!< true is terminal => one lexeme, else children
                parse_node   *prev;     //!< for a child list
                parse_node   *next;     //!< for a child list
                parse_node   *parent;   //!< for a parse tree
                
                virtual ~parse_node() throw();
                explicit parse_node( const string &label_ref, lexeme *lx ) throw();
                
                lexeme     * lex() throw();       //!< lexeme if terminal==true
                child_list & children() throw();  //!< children if terminal==false
                
                //! put back lexemes into lexer and delete node
                static void restore( lexer &Lxr, parse_node *node ) throw();
                
				static parse_node *create( const string &label_ref, lexeme *lx ); //!< take care of lexeme in case of failure

            private:
                uint64_t wksp[ YOCTO_U64_FOR_ITEM(child_list) ];
                YOCTO_DISABLE_COPY_AND_ASSIGN(parse_node);
                lexeme * & __lex() throw();
                
            };
        }
    }
    
}


#endif
