#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/exception.hpp"
#include "yocto/lingua/lexical/plugin/cstring.hpp"
#include "yocto/lingua/lexical/plugin/cstring.hpp"
#include "yocto/lingua/lexical/plugin/end_of_line_comment.hpp"


#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            void xgen:: initialize( xlist &top_level )
            {
                xlist tmp;
                while(top_level.size)
                {
                    xnode *node = top_level.pop_front();
                    tmp.push_back(node);
                    if(1==htop(node->label()))
                    {
                        assert("LXR"==node->label());
                        const string lx_id = node->ch->head->lx->to_string(1,0);
                        std::cerr << "lexical rule id=" << lx_id << std::endl;
                        if(hres(lx_id)>=0)
                        {
                            //-- this is a reserved keyword, will be processed later
                            continue;
                        }
                        else
                        {
                            //-- this must be a plugin
                            load_plugin(lx_id,node);
                            delete tmp.pop_back();
                        }
                    }
                }
                top_level.swap_with(tmp);
            }

            void xgen::load_plugin(const string &id,const xnode *node)
            {
                std::cerr << "\tloading plugin<" << id << ">" << std::endl;
                assert("LXR"==node->label());
                const xlist &sub_list = *(node->ch);
                const xnode *sub      = sub_list.head->next;

                if("cstring" == id)
                {
                    if(2!=sub_list.size) throw exception("plugin<cstring> must have only 1 argument");
                    assert(sub);
                    if("ID"!=sub->label())
                    {
                        throw exception("plugin<cstring> must have a rule ID as argument");
                    }
                    const string rule_id = sub->lx->to_string();
                    std::cerr << "used as '" << rule_id << "'" << std::endl;
                    rule &r = xprs->term<lexical::cstring>(rule_id);
                    
                    return;
                }

            }

        }

    }

}
