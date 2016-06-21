#include "yocto/lingua/syntax/analyzer.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/compound.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/utils.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            analyzer:: ~analyzer() throw()
            {
            }
            
            analyzer:: analyzer(const grammar &g) :
            onTerm(NULL),
            onRule(NULL),
            max_label_length(0),
            output(NULL)
            {

                const size_t   nr = g.num_rules();
                vector<string> Terms(nr,as_capacity);
                vector<string> Rules(nr,as_capacity);

                for(const rule *r = g.top_level();r;r=r->next)
                {
                    if(r->flags==property::jettison)
                        continue;

                    switch(r->uuid)
                    {
                        case terminal::UUID:
                            Terms.push_back(r->label);
                            max_label_length = max_of(max_label_length,r->label.size());
                            break;
                            
                        case aggregate::UUID:
                            Rules.push_back(r->label);
                            max_label_length = max_of(max_label_length,r->label.size());
                            break;

                        default:
                            break;
                    }
                }

                quicksort(Terms);
                quicksort(Rules);
                std::cerr << "|_Terms" << std::endl;
                for(size_t i=1;i<=Terms.size();++i)
                {
                    std::cerr << "  |_" << Terms[i] << std::endl;
                }
                std::cerr << "|_Rules" << std::endl;
                for(size_t i=1;i<=Rules.size();++i)
                {
                    std::cerr << "  |_" << Rules[i] << std::endl;
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

            void analyzer:: walk(const xnode *node, ios::ostream *fp)
            {
                depth  = 0;
                output = fp;
                size_t ns = 0;
                __walk(node,ns);
            }


            void analyzer:: emit(const string &label)
            {
                assert(output);
                const size_t lsize = label.size();
                output->append( label.c_str(), lsize );
                for(size_t i=lsize;i<max_label_length;++i) output->write(' ');
            }


            void analyzer:: __walk( const xnode *node, size_t &ns)
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
                        }
                        output->write('\n');
                    }
                    else
                    {
                        term_proc   *pProc   = NULL;
                        const string content = node->lx->to_string();
                        if(pProc)
                        {
                            (*pProc)(label,content);
                        }
                        else
                        {
                            std::cerr << "// +" << label << " '" << content << "'" << std::endl;
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
                        rule_proc *pCall = NULL;
                        if(pCall)
                        {
                            (*pCall)(label,local_ns);
                        }
                        else
                        {
                            std::cerr << "// @" << label << " /" << local_ns << std::endl;
                        }
                    }
                }
                ++ns;
            }


        }
    }
}

