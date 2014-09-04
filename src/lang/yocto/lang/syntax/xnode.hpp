#ifndef YOCTO_LANG_SYNTAX_XNODE_INCLUDED
#define YOCTO_LANG_SYNTAX_XNODE_INCLUDED 1

#include "yocto/lang/lexer.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            //! a syntax node, terminal or not
            /**
             - if terminal: smart pointer to a lexeme
             - if !terminal: holds some children
             */
            
            class xnode : public object
            {
            public:
                typedef core::list_of<xnode> child_list;
               
                const string &label;    //!< refering to the creating rule
                xnode        *next;     //!< for children
                xnode        *prev;     //!< for children
                xnode        *parent;   //!< for tree structure
                const bool    terminal; //!< terminal or not
                
                virtual ~xnode() throw();
                
                child_list       &children() throw();
                const child_list &children() const throw();
                lexeme *         &lxm() throw();
                const lexeme *   &lxm() const throw();
                
                //!create a new terminal node
                static xnode *create( const string &rule_label, lexeme *lx);
                
                //! create a new non terminal node
                static xnode *create( const string &rule_label );
                
                void add( xnode *child ) throw();       //!< register a new child
                void graphviz( const string &fn) const; //!< make a GraphViz file
                void graphviz( const char   *fn) const; //!< make a GraphViz file
                
            private:
                explicit xnode(const string &rule_label, lexeme *lx) throw();
                explicit xnode(const string &rule_label) throw();
                void viz( ios::ostream &fp ) const;
                
                uint64_t wksp[YOCTO_U64_FOR_ITEM(child_list)];
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
            };
        }
        
    }
}

#endif

