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
                        case 0: assert("RULE"==node->label());
                        {
                            if(is_alias(node))
                            {
                                delete tmp.pop_back();
                            }
                        } break;

                        case 1: assert("LXR" ==node->label());
                        {
                            const string id = node->ch->head->lx->to_string(1,0); // remove '@'
                            if(hres(id)<0)
                            {
                                // this is not a reserved keyword
                                load_plugin(id, node); // make a plugin
                                delete tmp.pop_back(); // remove from top level
                            }
                        } break;

                        case 2: assert("SEM" == node->label() );
                        {
                            const xnode * sub = node->ch->head;
                            const string  id  = sub->lx->to_string(1,0); // remove '$'
                            //std::cerr << "\t\twill process ''" << id << "''" << std::endl;
                            check_semantic(id,sub->next);
                            delete tmp.pop_back();
                        } break;


                        default:
                            throw exception("xgen.initialize(unexpected '%s')", node->label().c_str());
                    }

                }
                top_level.swap_with(tmp);
            }


            void xgen:: check_semantic(const string &id,
                                       const xnode  *node)
            {
                //______________________________________________________________
                //
                // choosing where to store info
                //______________________________________________________________
                ordered<string> *target = NULL;

                if( "no_single" == id )
                {
                    target = & no_single;
                }

                if(!target)
                {
                    throw exception("xgen.check_semantic(unhandled modifier '%s')", id.c_str());
                }

                //______________________________________________________________
                //
                // storing
                //______________________________________________________________
                for(;node;node=node->next)
                {
                    const string ruleID = node->lx->to_string();
                    YXGEN_OUT("\t\t" << id << " <= " << ruleID );
                    if( !target->insert(ruleID) )
                    {
                        throw exception("xgen.check_semantic(multiple rule '%s' for ''%s'')", ruleID.c_str(), id.c_str());
                    }
                }

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
                    return;
                }


                throw exception("unknown plugin<%s>", plg.c_str());
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


            bool xgen:: is_alias(const xnode *node)
            {
                assert("RULE"==node->label());
                const xlist &info = *(node->ch);
                if(2==info.size)
                {
                    const string &kind = info.tail->label();
                    if(kind=="RXP")
                    {
                        const string label = info.head->lx->to_string();
                        YXGEN_OUT("--> '" << label << "' is a regexp alias!");
                        const string expr  = info.tail->lx->to_cstring();
                        rule &alias = xprs->terminal(label, expr);
                        rule_ptr q(&alias);
                        if(!rules.insert(q))
                        {
                            throw exception("unexpected multiple regexp alias '%s'", label.c_str());
                        }
                        return true;
                    }

                    if(kind=="RAW")
                    {
                        const string label = info.head->lx->to_string();
                        YXGEN_OUT("--> '" << label << "' is a simple alias!");
                        const string content = info.tail->lx->to_string();
                        rule &alias = xprs->univocal(label,content);
                        rule_ptr q(&alias);
                        if(!rules.insert(q))
                        {
                            throw exception("unexpected multiple simple alias '%s'", label.c_str());
                        }
                        return true;
                    }
                    
                    
                }
                return false;
            }
        }
        
    }
    
}
