#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/exception.hpp"
#include "yocto/lingua/lexical/plugin/cstring.hpp"
#include "yocto/lingua/lexical/plugin/rstring.hpp"


#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            void xgen:: initialize( xlist &top_level )
            {
                YXGEN_OUT("initializing top level rules");
                xlist tmp;
                while(top_level.size)
                {
                    xnode *node = top_level.pop_front();
                    tmp.push_back(node);
                    switch(htop(node->label()))
                    {
                        case 0: assert("RULE"==node->label()); break;

                        case 1: assert("LXR" ==node->label());
                        {
                            const string id = node->ch->head->lx->to_string(1,0); // remove '@'
                            if(hres(id)<0)
                            {
                                // this is not a reserved keyword
                                load_plugin(id, node); // make a plugin
                                delete tmp.pop_back(); // remove from top level
                            }
                        }
                            break;

                        default:
                            throw exception("xgen.initialize(unexpected '%s')", node->label().c_str());
                    }

                }
                top_level.swap_with(tmp);
            }

            void xgen::load_plugin(const string &id, xnode *node)
            {
                assert("LXR"==node->label());

                xlist &args = *(node->ch);
                assert(args.size>=2);
                delete args.pop_front();
                assert(args.size>=1);
                assert(args.head->terminal);
                const string plg = args.head->lx->to_string();
                YXGEN_OUT("loading plugin <" << plg << "> as " << id);
                
                if("cstring" == plg)
                {
                    ld_cstring(id,args);
                    return;
                }

                if("rstring" == plg)
                {
                    ld_rstring(id,args);
                }

            }

            void xgen:: ld_cstring(const string &id, const xlist &args)
            {
                if(1!=args.size)       throw exception("plugin<cstring> must have only 1 argument");
                rule &r = xprs->term<lexical::cstring>(id);
                rule_ptr p(&r);
                if(!rules.insert(p))
                {
                    throw exception("plugin<cstring>: unexpected multiple '%s'", id.c_str());
                }
            }


            void xgen:: ld_rstring(const string &id, const xlist &args)
            {
                if(1!=args.size)       throw exception("plugin<rstring> must have only 1 argument");
                rule &r = xprs->term<lexical::cstring>(id);
                rule_ptr p(&r);
                if(!rules.insert(p))
                {
                    throw exception("plugin<rstring>: unexpected multiple '%s'", id.c_str());
                }
            }
            


        }

    }

}
