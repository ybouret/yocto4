#include "yocto/lingua/syntax/tree-walker.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"
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
            uuids(),
            max_label_length(0),
            output(0)
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
                                max_label_length = max_of(max_label_length,r->label.size());
                            }
                            break;

                        default:
                            break;

                    }

                }
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


#include <iomanip>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            void tree_walker:: emit(const string &label)
            {
                assert(output);
                const size_t lsize = label.size();
                output->append( label.c_str(), lsize );
                for(size_t i=lsize;i<max_label_length;++i) output->write(' ');
            }



            void tree_walker:: __walk( const XNODE *node, size_t &ns )
            {
                assert(node);
                const string &label  = node->label();

                if(node->terminal)
                {
                    if(output)
                    {
                        output->write('+');
                        emit(label);
                        if(property::standard==node->origin->flags)
                        {
                            const string content = node->lx->to_cstring();
                            output->write('=');
                            output->write('\'');
                            output->append(content.c_str(),content.size());
                            output->write('\'');
                            output->write('\n');
                        }
                    }
                    else
                    {
                        term_proc *pProc = term_procs.search(label);
                        if(pProc)
                        {
                            const string content = node->lx->to_string();
                            (*pProc)(content);
                        }
                    }
                }
                else
                {

                    ++depth;
                    size_t local_ns=0;
                    for(const xnode *sub = node->ch->head;sub;sub=sub->next)
                    {
                        __walk(sub,local_ns);
                    }
                    --depth;
                    //YTREE_INDENT(); std::cerr << "CALL: " << label << "(" << local_ns << ")" << std::endl;
                    if(output)
                    {
                        output->write('@');
                        emit(label);
                        output->write('/');
                        (*output)("%u",unsigned(local_ns));
                        output->write('\n');
                    }
                    else
                    {
                        rule_proc *pCall = rule_procs.search(label);
                        if(pCall)
                        {
                            (*pCall)(local_ns);
                        }
                    }
                }
                ++ns;
            }
            
            
            void tree_walker::walk(const XNODE *node, ios::ostream *fp)
            {
                depth     = 0;
                output    = fp;
                size_t ns = 0;
                __walk(node,ns);
            }
            
        }
    }
}
