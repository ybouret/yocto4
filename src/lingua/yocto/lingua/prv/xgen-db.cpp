#include "yocto/lingua/prv/xgen.hpp"
#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            void xgen:: build_syntax_databases(const xnode *node)
            {
                if(node->terminal)
                {
                    const string  content = node->lx->to_string();
                    const string &label   = node->origin->label;
                    switch(h_db(label))
                    {
                        case 0: {
                            assert("ID"==label);
                            std::cerr << "Found ID='" << content << "'" << std::endl;
                            //syntax::aggregate  &a = xprs->agg(content);
                            //const agg_ptr       p(&a);

                        } break;

                        case 1: {
                            assert("RXP"==label);
                            std::cerr << "Found RXP='" << content << "'" << std::endl;
                        } break;

                        case 2: {
                            assert("RAW"==label);
                            std::cerr << "Found RAW='" << content << "'" << std::endl;
                        } break;


                        default:
                            break;
                    }

                }
                else
                {
                    for(const xnode *sub = node->ch->head; sub; sub=sub->next)
                    {
                        build_syntax_databases(sub);
                    }
                }

            }

        }

    }

}