#include "yocto/lingua/syntax/tree-walker.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            tree_walker:: ~tree_walker() throw()
            {
            }

            tree_walker:: tree_walker(const grammar &G) :
            depth(0),
            rule_procs(),
            term_procs(),
            uuids()
            {
                size_t nr = 0;
                size_t nt = 0;
                for(const rule *r = G.top_level();r;r=r->next)
                {
                    bool is_agg = false;
                    switch(r->uuid)
                    {
                        case aggregate::UUID: is_agg = true;
                        case terminal:: UUID:
                            if( property::jettison != r->flags )
                            {
                                if(!uuids.insert(r->label,r->uuid))
                                {
                                    throw exception("tree_walker: unexpected multiple rule '%s'", r->label.c_str());
                                }
                                if(is_agg) ++nr; else ++nt;
                            }
                            break;

                        default:
                            break;

                    }

                }
                std::cerr << "#rule =" << nr << std::endl;
                std::cerr << "#term =" << nt << std::endl;
                rule_procs.reserve(nr);
                term_procs.reserve(nt);
            }



            uint32_t tree_walker:: uuid_for(const string &label) const
            {
                const uint32_t *pUUID = uuids.search(label);
                if(!pUUID)
                {
                    throw exception("tree_walker: no uuid for '%s'", label.c_str());
                }
                return *pUUID;
            }

            void tree_walker:: check_is_term(const string &label) const
            {
                if(terminal::UUID != uuid_for(label))
                {
                    throw exception("tree_walker: illegal terminal '%s'", label.c_str());
                }
            }

            void tree_walker:: check_is_rule(const string &label) const
            {
                if(aggregate::UUID != uuid_for(label))
                {
                    throw exception("tree_walker: illegal rule '%s'", label.c_str());
                }
            }

        }

    }

}


namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

#define YTREE_INDENT() do { for(int nt=0;nt<depth;++nt) { std::cerr << "  "; } } while(false)

            void tree_walker:: __walk( const XNODE *node )
            {
                assert(node);
                const string &label  = node->label();
                
                if(node->terminal)
                {
                    YTREE_INDENT(); std::cerr << "TERM: " << label << " : '" << *(node->lx) << "'" << std::endl;
                    term_proc *pProc = term_procs.search(label);
                    if(pProc)
                    {
                        const string content = node->lx->to_string();
                        (*pProc)(content);
                    }
                }
                else
                {
                    
                    ++depth;
                    for(const xnode *sub = node->ch->head;sub;sub=sub->next)
                    {
                        __walk(sub);
                    }
                    --depth;
                    YTREE_INDENT(); std::cerr << "CALL: " << label << std::endl;
                    rule_proc *pCall = rule_procs.search(label);
                    if(pCall)
                    {
                        (*pCall)();
                    }
                }
            }


            void tree_walker::walk(const XNODE *node)
            {
                depth = 0;
                __walk(node);
            }

        }
    }
}
