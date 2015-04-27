#ifndef YOCTO_LANG_SYNTAX_XNODE_INCLUDED
#define YOCTO_LANG_SYNTAX_XNODE_INCLUDED 1

#include "yocto/lang/lexeme.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            //! a node to store lexemes in AST
            class xnode
            {
            public:
                enum type_t
                {
                    is_terminal,
                    is_internal
                };

                xnode       *next;
                xnode       *prev;
                const type_t type;

                typedef core::list_of_cpp<xnode> leaves;

                ~xnode() throw();

                static xnode *term(lexeme *l); //!< new terminal
                static xnode *leaf();


                YOCTO_MAKE_OBJECT
            private:
                xnode(type_t t) throw();

                union
                {
                    lexeme   *lx;
                    leaves   *ch;
                };
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
            };

        }
    }
}

#endif
