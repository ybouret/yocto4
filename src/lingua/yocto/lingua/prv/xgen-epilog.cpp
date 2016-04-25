#include "yocto/lingua/prv/xgen.hpp"
#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {


            void xgen:: univocal2jettison() throw()
            {
                const rules_db::iterator __end = rules.end();
                for( rules_db::iterator i = rules.begin(); i!=__end; ++i)
                {
                    rule &r = **i;
                    if(aggregate::UUID!=r.uuid) continue;
                    //YXGEN_OUT("checking '" << r.label << "'");
                    rule::meta_list &m = r.as<aggregate>().members;
                    if(m.size>=2)
                    {
                        for(rule::meta_node *node = m.head;node;node=node->next)
                        {
                            const rule *sub_addr = node->addr;
                            rule       &sub      = *(rule *)sub_addr;
                            if(terminal::UUID==sub.uuid && property::univocal==sub.flags)
                            {
                                YXGEN_OUT("\tjettison '" << sub.label << "'" );
                                sub.flags = property::jettison;
                            }
                        }
                    }
                }

            }

        }

    }

}

