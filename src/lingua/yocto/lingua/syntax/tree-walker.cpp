#include "yocto/lingua/syntax/tree-walker.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            tree_walker:: ~tree_walker() throw()
            {
            }

            tree_walker:: tree_walker() :
            depth(0),
            calls(),
            procs()
            {
            }




        }

    }

}

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

#define YTREE_INDENT() do { for(int nt=0;nt<depth;++nt) { std::cerr << "  "; } } while(false)

            void tree_walker:: on( const XNODE *node )
            {
                assert(node);
                const string &label  = node->label();
                
                if(node->terminal)
                {
                    YTREE_INDENT(); std::cerr << "TERM: " << label << " : '" << *(node->lx) << "'" << std::endl;
                    proc_type *pProc = procs.search(label);
                    if(pProc)
                    {
                        const string content = node->lx->to_string();
                        (*pProc)(content);
                    }
                }
                else
                {
                    YTREE_INDENT(); std::cerr << "CALL: " << label << std::endl;
                    call_type *pCall = calls.search(label);
                    if(pCall)
                    {
                        (*pCall)();
                    }

                    ++depth;
                    for(const xnode *sub = node->ch->head;sub;sub=sub->next)
                    {
                        on(sub);
                    }
                    --depth;
                }
            }


            void tree_walker::walk(const XNODE *node)
            {
                depth = 0;
                on(node);
            }

        }
    }
}
