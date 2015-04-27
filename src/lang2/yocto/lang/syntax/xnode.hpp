#ifndef YOCTO_LANG_SYNTAX_XNODE_INCLUDED
#define YOCTO_LANG_SYNTAX_XNODE_INCLUDED 1

#include "yocto/lang/lexeme.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            class rule;

            //! a node to store lexemes in AST
            class xnode
            {
            public:
                typedef int property;

                xnode         *next;
                xnode         *prev;
                xnode         *parent;
                const string  &label;    //!< name of the rule
                bool           terminal; //!< true if is terminal
                const property modifier; //!<

                typedef core::list_of_cpp<xnode> leaves;

                ~xnode() throw();

                static xnode *term(const rule &r, lexeme *l); //!< new terminal
                static xnode *leaf(const rule &r);

                lexeme &lex() throw();
                leaves &children() throw();
                void    append(xnode *node) throw(); //!< append to children

                //! restore all lexemes and delete node
                static  void restore(xnode *node, l_list &lexemes) throw();

                void viz( ios::ostream &fp ) const;
                void graphivz( const string &filename ) const;

                YOCTO_MAKE_OBJECT

            private:
                xnode(const rule &r, bool flag, const property ppty) throw();

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
