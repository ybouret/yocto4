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
           
           
            enum property
            {
                //______________________________________________________________
                //
                // for terminals
                //______________________________________________________________
                is_standard,         //!< a standard node
                is_univocal    = 1,  //!< univocal, erase content
                is_jettison    = 2,  //!< syntax only
                
                //______________________________________________________________
                //
                // for non terminals
                //______________________________________________________________
                is_merging_all = 4,  //!< a temporary list, merge with parent if possible
                is_merging_one = 8   //!< for grouping rules, merge is size==1
            };
            
            //! a syntax node, terminal or not
            /**
             - if terminal: smart pointer to a lexeme
             - if !terminal: holds some children
             */
            class xnode : public object
            {
            public:
                typedef core::list_of_cpp<xnode> child_list;
                
                const string    &label;    //!< refering to the creating rule
                xnode           *next;     //!< for children
                xnode           *prev;     //!< for children
                xnode           *parent;   //!< for tree structure
                const bool       terminal; //!< terminal or not
                const property   modifier; //!< be more precise
                
                virtual ~xnode() throw();
                
                child_list       &children() throw();
                const child_list &children() const throw();
                size_t            count()    const throw(); //!< #children
                
                //! register a new child with no parent/next/prev.
                void   add( xnode *child ) throw();
                
                //! children().pop_front() and set no parent
                xnode *pop() throw();
                
                lexeme *         &lxm() throw();
                const lexeme *   &lxm() const throw();
                
                //!create a new terminal node
                static xnode *create( const string &rule_label, lexeme *lx, const property ppty);
                
                //! create a new non terminal node
                static xnode *create( const string &rule_label, const property ppty);
                
                
                
                //! make a GraphViz file
                void graphviz( const string &fn) const;
                
                //! make a GraphViz file
                void graphviz( const char   *fn) const;
                
                //! restore a node to the lexer and delete it
                static void restore( lexer &Lexer, xnode *node ) throw();
                
                //! cleanup
                static xnode *ast( xnode *node ) throw();
                
            private:
                explicit xnode(const string &rule_label, lexeme *lx, const property ppty) throw();
                explicit xnode(const string &rule_label, const property ppty) throw();
                void viz( ios::ostream &fp ) const;
                void clr() throw();
                
                uint64_t wksp[YOCTO_U64_FOR_ITEM(child_list)];
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
            };
            
            typedef xnode *         xtree;
            typedef auto_ptr<xnode> x_ptr;
            
        }
        
    }
}

#endif

