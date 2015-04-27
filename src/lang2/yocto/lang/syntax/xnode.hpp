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

                xnode       *next;
                xnode       *prev;
                xnode       *parent;
                bool         is_term;

                typedef core::list_of_cpp<xnode> leaves;

                ~xnode() throw();

                static xnode *term(lexeme *l); //!< new terminal
                static xnode *leaf();

                lexeme &lex() const;
                leaves &children() const;
                void    append(xnode *node) throw(); //!< append to children


                YOCTO_MAKE_OBJECT

            private:
                xnode(bool flag) throw();

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
