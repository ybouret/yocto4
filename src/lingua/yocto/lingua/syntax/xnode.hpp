#ifndef YOCTO_LINGUA_SYNTAX_XNODE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_XNODE_INCLUDED 1

#include "yocto/lingua/lexeme.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            //! a node to build AST
            /**
             may hold a terminal lexeme or a list of xnode
             */
            class xnode 
            {
            public:
                typedef core::list_of_cpp<xnode> list_type;

                xnode     *next;
                xnode     *prev;
                xnode     *parent;
                const bool terminal;
                union
                {
                    lexeme    *lx;
                    list_type *leaves;
                };

                

                YOCTO_MAKE_OBJECT
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
            };
            
        }
    }
}


#endif

