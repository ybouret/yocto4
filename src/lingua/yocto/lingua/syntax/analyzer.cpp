#include "yocto/lingua/syntax/analyzer.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/compound.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/utils.hpp"

#include <iostream>
#include "yocto/ios/graphviz.hpp"
#include "yocto/exception.hpp"

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
            hterm(),
            hrule(),
            depth(0),
            max_label_length(0),
            output(NULL)
            {

                vector<string> Terms;
                vector<string> Rules;
                max_label_length = g.collect(Terms,Rules);

                int j = 0;
                for(size_t i=1;i<=Terms.size();++i)
                {
                    hterm.insert(Terms[i], ++j);
                }
                hterm.optimize();

                hterm.graphviz("hterm.dot");
                ios::graphviz_render("hterm.dot");

                for(size_t i=1;i<=Rules.size();++i)
                {
                    hrule.insert(Rules[i], ++j);
                }
                hrule.optimize();

                hrule.graphviz("hrule.dot");
                ios::graphviz_render("hrule.dot");


                std::cerr << "|_Terms" << std::endl;
                for(size_t i=1;i<=Terms.size();++i)
                {
                    std::cerr << "  |_" << Terms[i];
                    for(size_t k=Terms[i].size();k<=max_label_length;++k) std::cerr << ' ';
                    std::cerr << " @" << hterm(Terms[i]) << std::endl;
                }
                
                std::cerr << "|_Rules" << std::endl;
                for(size_t i=1;i<=Rules.size();++i)
                {
                    std::cerr << "  |_" << Rules[i];
                    for(size_t k=Rules[i].size();k<=max_label_length;++k) std::cerr << ' ';
                    std::cerr << " @" << hrule(Rules[i]) << std::endl;
                }
                
            }

            int analyzer:: hash_term(const string &label) const
            {
                const int ans = hterm(label);
                if(ans<0)
                    throw exception("syntax::analyzer(unknown terminal '%s')", label.c_str());
                return ans;
            }

            int analyzer:: hash_rule(const string &label) const
            {
                const int ans = hrule(label);
                if(ans<0)
                    throw exception("syntax::analyzer(unknown rule '%s')", label.c_str());
                return ans;
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
                        term_proc   *pProc   = onTerm.__get();
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
                        rule_proc *pCall = onRule.__get();
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

