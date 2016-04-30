#ifndef YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lingua/syntax/xnode.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            class tree_walker : public object
            {
            public:
                typedef xnode XNODE;
                virtual ~tree_walker() throw();
                explicit tree_walker() throw();

                void walk(const XNODE *node);

            protected:
                int  depth;
                void on( const XNODE *node );

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree_walker);
            };
        }
    }
}

#endif
