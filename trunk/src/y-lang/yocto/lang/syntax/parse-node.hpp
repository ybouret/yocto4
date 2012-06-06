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
            
            class parse_node : public object
            {
            public:
                typedef core::list_of<parse_node> child_list;
                const string  &label;    //!< reference to the corresponding rule label
                parse_node    *prev;     //!< for a child list
                parse_node    *next;     //!< for a child list
                parse_node    *parent;   //!< for a parse tree
                
                virtual ~parse_node() throw();
                explicit parse_node( const string &label_ref, lexeme *lx ) throw();
                
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
				static parse_node *create( const string &label_ref, lexeme *lx );
                
                void  viz( ios::ostream &fp ) const;
                void  graphviz( const string &id, ios::ostream &fp ) const;
                void  graphviz( const char   *id, ios::ostream &fp ) const;
              
                void  compress() throw();
                
            private:
                uint64_t wksp[ YOCTO_U64_FOR_ITEM(child_list) ];
                YOCTO_DISABLE_COPY_AND_ASSIGN(parse_node);
                lexeme * & __lex() throw();
            public:
                static const size_t   data_size   = sizeof(uint64_t) * ( YOCTO_U64_FOR_ITEM(child_list) );
                static const size_t   list_size   = sizeof(child_list);
                static const uint16_t discardable = 0x0001;
                const uint16_t terminal;
                uint16_t       flags;
            };
        }
    }
    
}


#endif
