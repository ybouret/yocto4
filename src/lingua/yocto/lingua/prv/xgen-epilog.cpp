#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/exception.hpp"
#include "yocto/lingua/syntax/joker.hpp"
#include "yocto/sequence/vector.hpp"

#include <iostream>
#include <cstring>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            bool xgen:: is_internal(const rule &r) throw()
            {
                return 0 != strchr(r.label.c_str(),rule::internal_char);
            }

            static inline
            void __univocal2jettison(rule::meta_list &m, const bool verbose)
            {
                assert(m.size>=2);
                for(rule::meta_node *node = m.head;node;node=node->next)
                {
                    const rule *sub_addr = node->addr;
                    rule       &sub      = *(rule *)sub_addr;
                    if(terminal::UUID==sub.uuid && property::univocal==sub.flags)
                    {
                        YXGEN_OUT("|_jettison '" << sub.label << "'" );
                        sub.flags = property::jettison;
                    }
                }
            }


            static inline
            void optimize_aggregate( aggregate &p, const bool verbose)
            {
                rule::meta_list &m = p.members;
                switch(m.size)
                {
                    case 0: break;
                    case 1: break;
                    default: __univocal2jettison(m,verbose);  break;
                }
            }

            static inline
            const rule *optimize_joker(const rule *jk,sequence<rule*> &to_remove,const bool verbose)
            {
                assert(jk);
                YXGEN_OUT("|_checking joker '" << jk->label << "'");
                if(jk->uuid==aggregate::UUID)
                {
                    const aggregate &a = jk->as<aggregate>();
                    if( xgen::is_internal(a) )
                    {
                        const rule::meta_list &m = a.members;
                        if(m.size==1)
                        {
                            //YXGEN_OUT("will fusion '" << a.label << "'");
                            to_remove.push_back((rule*)jk);
                            return m.head->addr;
                        }
                    }
                }
                return jk;
            }

            void xgen:: optimize()
            {
                const rules_db::iterator __end = rules.end();
                vector<rule *>           to_remove(rules.size(),as_capacity);
                for( rules_db::iterator i = rules.begin(); i!=__end; ++i)
                {
                    rule &r = **i;
                    
                    switch(r.uuid)
                    {
                        case aggregate::UUID: optimize_aggregate( r.as<aggregate>(), verbose); break;

                        case at_least::UUID: {
                            at_least   &J  = r.as<at_least>();
                            J.jk = optimize_joker(J.jk,to_remove,verbose);
                        } break;

                        case optional::UUID: {
                            optional &J  = r.as<optional>();
                            J.jk = optimize_joker(J.jk,to_remove,verbose);
                        } break;

                        default:
                            break;
                    }
                }

                for(size_t i=to_remove.size();i>0;--i)
                {
                    rule *p = to_remove[i]; assert(p!=NULL);
                    YXGEN_OUT("|____removing '" << p->label << "'");
                    assert(xprs->rules.owns(p));
                    delete xprs->rules.unlink(p);
                }
            }



        }

    }

}
