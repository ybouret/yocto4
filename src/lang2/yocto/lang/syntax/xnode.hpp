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

            enum  property
            {
                standard,   //!< untouched in tree
                univocal,   //!< content is deleted
                jettison,   //!< node is deleted
                mergeAll,   //!< content is merged with parent's
                mergeOne    //!< content is merged if only 1 child (for grouping)
            };

            //! a node to store lexemes in AST
            class xnode
            {
            public:
                xnode         *next;
                xnode         *prev;
                xnode         *parent;
                const string  &label;    //!< name of the rule
                bool           terminal; //!< true if is terminal
                const property modifier; //!< assigned by syntax rule

                typedef core::list_of_cpp<xnode> leaves;

                ~xnode() throw();

                static xnode *term(const rule &r, lexeme *l, const property ppty=standard); //!< new terminal
                static xnode *leaf(const rule &r, const property ppty = standard);

                lexeme &lex()      throw();
                leaves &children() throw();
                void    append(xnode *node) throw(); //!< append to children
                xnode  *pop() throw();               //!< head of children
                
                //! restore all lexemes and delete node
                static  void restore(xnode *node, l_list &lexemes) throw();

                //! output a graphiv representation
                void graphivz( const string &filename ) const;


                //! syntax tree rewrite
                static xnode *AST( xnode *node ) throw();

                YOCTO_MAKE_OBJECT


            private:
                xnode(const rule &r, bool flag, const property ppty);

                union
                {
                    lexeme   *lx;
                    leaves   *ch;
                };
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
                void viz( ios::ostream &fp ) const;
            };

        }
    }
}

#endif
