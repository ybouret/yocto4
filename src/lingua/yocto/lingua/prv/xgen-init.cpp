#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/exception.hpp"
#include "yocto/lingua/lexical/plugin/cstring.hpp"
#include "yocto/lingua/lexical/plugin/rstring.hpp"
#include "yocto/lingua/lexical/plugin/end_of_line_comment.hpp"
#include "yocto/lingua/lexical/plugin/block-comment.hpp"


#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            void xgen:: initialize( xlist &top_level )
            {
                std::cerr << "-- initializing" << std::endl;
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
                                load_plugin(id, node);
                                delete tmp.pop_back();
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
                std::cerr << "\tloading plugin<" << id << ">" << std::endl;
                assert("LXR"==node->label());
                xlist &args = *(node->ch);
                assert(args.size>0);
                delete args.pop_front();

                if("cstring" == id)
                {
                    ld_cstring(args);
                    return;
                }

            }

            void xgen:: ld_cstring(const xlist &args)
            {
                if(1!=args.size)       throw exception("plugin<cstring> must have only 1 argument");
                const xnode *arg = args.head;
                if("ID"!=arg->label()) throw exception("plugin<cstring> must have a rule ID as argument");
                const string rule_id = arg->lx->to_string();
                std::cerr << "used as '" << rule_id << "'" << std::endl;
                rule &r = xprs->term<lexical::cstring>(rule_id);
                rule_ptr p(&r);
                if(!rules.insert(p))
                {
                    throw exception("plugin<cstring>: unexpected multiple '%s'", rule_id.c_str());
                }

            }


            void xgen:: ld_rstring(const xlist &args)
            {
                if(1!=args.size)       throw exception("plugin<rstring> must have only 1 argument");
                const xnode *arg = args.head;
                if("ID"!=arg->label()) throw exception("plugin<rstring> must have a rule ID as argument");
                const string rule_id = arg->lx->to_string();
                std::cerr << "used as '" << rule_id << "'" << std::endl;
                rule &r = xprs->term<lexical::rstring>(rule_id);
                rule_ptr p(&r);
                if(!rules.insert(p))
                {
                    throw exception("plugin<rstring>: unexpected multiple '%s'", rule_id.c_str());
                }

            }
            


        }

    }

}
