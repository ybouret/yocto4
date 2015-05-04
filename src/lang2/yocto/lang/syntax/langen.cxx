#include "yocto/lang/syntax/langen.hxx"
#include "yocto/exception.hpp"
#include "yocto/lang/syntax/aggregate.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            LanGen:: ~LanGen() throw() {}


            LanGen:: LanGen(const xnode *node ) :
            root(node),
            P(0)
            {
                assert(root!=NULL);
                P.reset( new parser("dummy","main") );
                collect(root);
            }

            void LanGen:: collect(const xnode *node)
            {
                if(node->terminal)
                {
                    // a non terminal rule
                    if(node->label=="ID")
                    {
                        const string r_id = node->lex().to_string();
                        if(!P->has(r_id))
                        {
                            (void) P->agg(r_id);
                        }
                        return;
                    }

                    // a terminal regexp
                    if(node->label=="RX")
                    {
                        const string t_id = node->lex().to_string();
                        if(!P->has(t_id))
                        {
                            (void) P->term(t_id.c_str(), t_id.c_str());
                        }
                        return;
                    }

                    std::cerr << "\tunregistered " << node->label << "(" << node->lex() << ")" << std::endl;

                }
                else
                {
                    for(const xnode *child = node->children().head; child;child=child->next)
                    {
                        collect(child);
                    }
                }

            }


        }
    }
}

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

        }

    }

}


