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
                xnode        *next;
                xnode        *prev;
                xnode        *parent;
                const rule   *origin;

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

                YOCTO_MAKE_OBJECT
            private:
                xnode();
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
            };
            
        }
    }
}


#endif

