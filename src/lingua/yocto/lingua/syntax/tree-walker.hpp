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

            typedef functor<void,null_type>            call_type;
            typedef functor<void,TL1(const string &)>  proc_type;
            
            class tree_walker : public object
            {
            public:
                typedef xnode     XNODE;
                typedef call_type CALL_TYPE;
                typedef proc_type PROC_TYPE;
                typedef glossary<call_type> callDB;
                typedef glossary<proc_type> procDB;

                virtual ~tree_walker() throw();
                explicit tree_walker();

                void walk(const XNODE *node);

            protected:
                int        depth;
                callDB     calls;
                procDB     procs;
                
                void on( const XNODE *node );


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree_walker);
            };
        }
    }
}

#endif
