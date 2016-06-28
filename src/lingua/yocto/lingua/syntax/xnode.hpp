#ifndef YOCTO_LINGUA_SYNTAX_XNODE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_XNODE_INCLUDED 1

#include "yocto/lingua/lexer.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            class rule;
            class                            xnode;
            typedef core::list_of_cpp<xnode> xlist;

            //! a node to build AST
            /**
             may hold a terminal lexeme or a list of xnode
             */
            class xnode 
            {
            public:
                xnode        *next;   //!< for xlist
                xnode        *prev;   //!< for xlist
                xnode        *parent; //!< for tree ops
                const rule   *origin; //!< hold label, flags...

                union {
                    lexeme *lx;
                    xlist  *ch;
                };
                const bool terminal;

                ~xnode() throw();

                void         push_back(  xnode *node ) throw(); //!< push_back wrapper
                void         push_front( xnode *node ) throw(); //!< push_front wrapper
                xnode       *pop_back()  throw(); //!< pop_back wrapper
                xnode       *pop_front() throw(); //!< pop_front wrapper
                const xnode *head() const throw(); //!< R/O head wrapper
                const xnode *tail() const throw(); //!< R/O tail wrapper
                size_t size() const throw(); //!< 0 if terminal, ch->size otherwise

                //! rule is a terminal...
                static xnode * create( const rule &r, lexeme *l );

                //! rule is NOT a terminal
                static xnode * create( const rule &r);

                //! origin->label...
                const string   &label() const throw();

                //! return content to lexer
                static void back_to(lexer &lxr, xnode *node) throw();

                void viz( ios::ostream &fp ) const;          //!< for one node
                void graphviz(const string &filename) const; //!< from this node

                static xnode *ast(xnode *node) throw();

                YOCTO_MAKE_OBJECT

                void save(ios::ostream &fp) const;

            private:
                xnode();
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
            };
            
        }
    }
}


#endif

