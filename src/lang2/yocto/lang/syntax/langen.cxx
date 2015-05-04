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

            LanGen:: LanGen(const xnode *node ) :
            root(node),
            rules(),
            terms()
            {
                assert(root!=NULL);
                
            }

            void LanGen:: collect(const xnode *node)
            {
                if(node->terminal)
                {
                    // a non terminal rule
                    if(node->label=="ID")
                    {
                        const string r_id = node->lex().to_string();
                        if( !rules.search(r_id) )
                        {
                            std::cerr << "New ID: " << node->lex() << std::endl;
                            const pRule pR( new aggregate(r_id,syntax::standard) );
                            if(!rules.insert(pR))
                            {
                                throw exception("Failure in inserting '%s'", r_id.c_str());
                            }
                        }
                        return;
                    }

                    // a terminal regexp
                    if(node->label=="RX")
                    {
                        const string p_id = node->lex().to_string();
                        if( !terms.search(p_id) )
                        {
                            std::cerr << "New RX: " << node->lex() << std::endl;
                            terms.define(p_id, regexp(p_id, &terms) );
                        }
                        return;
                    }

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
