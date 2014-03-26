#ifndef YOCTO_LANG_SYNTAX_PARSE_NODE_INCLUDED
#define YOCTO_LANG_SYNTAX_PARSE_NODE_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/code/round.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        
        namespace syntax
        {
            enum node_property
            {
                is_regular     = 0,
                is_discardable = 1, //!< no semantic meaning
                is_specialized = 2, //!< univocal meaning, erase content
                is_merging_all = 4, //!< merge with parent's children
                is_merging_one = 8  //!< merge with parent's children if only one child
            };
            
            class parse_node : public object
            {
            public:
                typedef core::list_of<parse_node> child_list;
                const string  &label;    //!< reference to the corresponding rule label
                parse_node    *prev;     //!< for a child list
                parse_node    *next;     //!< for a child list
                parse_node    *parent;   //!< for a parse tree
                
                virtual ~parse_node() throw();
                explicit parse_node( const string &label_ref, lexeme *lx, node_property ppty ) throw();
                
                lexeme     * lex() throw();       //!< lexeme if terminal==true
                child_list & children() throw();  //!< children if terminal==false
                
                const lexeme     *lex() const throw();
                const child_list &children() const throw(); 
                
                //! put back lexemes into lexer and delete node
                static void restore( lexer &Lxr, parse_node *node ) throw();
                
                //! safe creation
                /**
                 take care of lexeme in case of failure
                 */
				static parse_node *create( const string &label_ref, lexeme *lx , node_property ppty );
                
                void  viz( ios::ostream &fp ) const;
                void  graphviz( const string &id, ios::ostream &fp ) const;
                void  graphviz( const char   *id, ios::ostream &fp ) const;
                void  out( ios::ostream &fp, size_t &depth ) const;
                void  output( ios::ostream &fp ) const;
              
                //! remove discardable, drop specialized content
                void  AST() throw();
                
                //! unlink from parent
                parse_node *unlink() throw();
                
                
            private:
                uint64_t wksp[ YOCTO_U64_FOR_ITEM(child_list) ];
                YOCTO_DISABLE_COPY_AND_ASSIGN(parse_node);
                lexeme * & __lex() throw();
            public:
                const uint16_t terminal; //!< 1 or 0
                const uint16_t flags;    //!< a node_property
            };
        }
    }
    
}


#endif
