#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            void xgen:: process_pty__level1(const xnode *node)
            {
                assert("PTY"==node->label);
                const xnode *ch = node->head();
                assert("PN"==ch->label);
                const string  pname = ch->content(1,0);
                ordered<string>       *IDB = 0;
                if("no_single"==pname)
                {
                    IDB = & no_single;
                    goto FILL_DB;
                }

                if("one_level"==pname)
                {
                    IDB = &one_level;
                    goto FILL_DB;
                }

            FILL_DB:
                if(!IDB)
                    throw exception("%s: unhandle property table '%s'", name, pname.c_str());
                
                for(ch=ch->next;ch;ch=ch->next)
                {
                    assert("ID"==ch->label);
                    const string id = ch->content();
                    if(! IDB->insert(id))
                    {
                        throw exception("%s: multiple rule '%s' in %s",name, id.c_str(), pname.c_str());
                    }
                }
            }

            void xgen::check_properties()
            {
                check_ppty_db(no_single);
                check_ppty_db(one_level);

                for(size_t i=no_single.size();i>0;--i)
                {
                    agg_ptr       *ppA = agg.search(no_single[i]); assert(ppA);
                    (property &)((**ppA).modifier) = mergeOne;
                }

            }


            void xgen::check_ppty_db(const pptyDB &db)
            {
                for(size_t i=db.size();i>0;--i)
                {
                    const string  &id  = db[i];
                    agg_ptr       *ppA = agg.search(id);
                    if(!ppA)
                    {
                        throw exception("%s: unregistered '%s' for ppty modification",name, id.c_str());
                    }
                }
            }


        }

    }
    
}

