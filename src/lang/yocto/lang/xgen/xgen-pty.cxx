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
                if("mergeOne"==pname)
                {
                    IDB = & mergeOneDB;
                    goto FILL_DB;
                }

                if("mergeAll"==pname)
                {
                    IDB = & mergeAllDB;
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
                check_ppty_db(mergeOneDB);
                check_ppty_db(mergeAllDB);

                for(size_t i=mergeOneDB.size();i>0;--i)
                {
                    agg_ptr       *ppA = agg.search(mergeOneDB[i]); assert(ppA);
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

