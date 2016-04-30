#ifndef YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/associative/glossary.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            typedef functor<void,TL1(const string&)> callback;
            
            class tree_walker : public object
            {
            public:
                typedef xnode    XNODE;
                typedef callback CALLBACK;
                typedef glossary<CALLBACK> procedures;

                virtual ~tree_walker() throw();
                explicit tree_walker() throw();

                void walk(const XNODE *node);

            protected:
                int        depth;
                procedures callbacks;
                void on( const XNODE *node );


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree_walker);
            };
        }
    }
}

#endif
