#include "yocto/lingua/syntax/grammar.hpp"

#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/joker.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            grammar:: ~grammar() throw()
            {
            }

#define Y_GRAMMAR_CTOR() name(id), rules(), nopt(0), nalt(0), nzom(0), noom(0)

            grammar:: grammar(const string &id) :
            Y_GRAMMAR_CTOR()
            {
            }

            void grammar:: check(const string &label) const
            {
                for(const rule *r = rules.head;r;r=r->next)
                {
                    if(label==r->label)
                    {
                        throw exception("[%s]: multiple rules '%s'", name.c_str(), label.c_str());
                    }
                }
            }

            rule & grammar:: decl_term(const string &label,const uint32_t flags)
            {
                check(label);
                terminal *tt = new terminal(label);
                tt->flags    = flags;
                rules.push_back(tt);
                return *tt;
            }

            
            rule & grammar:: opt(const rule &r)
            {
                assert(rules.owns(&r));
                const string label = vformat("opt%c%u",rule::internal_char,++nopt);
                optional    *tt    = new optional(label,&r);
                rules.push_back(tt);
                return *tt;
            }

            rule & grammar:: zero_or_more(const rule &r)
            {
                assert(rules.owns(&r));
                const string label = vformat(">=0%c%u",rule::internal_char,++nzom);
                at_least    *tt    = new at_least(label,&r,0);
                rules.push_back(tt);
                return *tt;
            }

            rule & grammar:: one_or_more(const rule &r)
            {
                assert(rules.owns(&r));
                const string label = vformat(">=1%c%u",rule::internal_char,++noom);
                at_least    *tt    = new at_least(label,&r,0);
                rules.push_back(tt);
                return *tt;
            }

            aggregate & grammar:: agg(const string &id, const uint32_t flags)
            {
                check(id);
                aggregate *tt = new aggregate(id);
                rules.push_back(tt);
                tt->flags     = flags;
                return *tt;
            }

            alternate & grammar:: alt()
            {
                const string label = vformat("alt%c%u",rule::internal_char,++nalt);
                alternate   *tt    = new alternate(label);
                rules.push_back(tt);
                return *tt;
            }

            void grammar:: top_level(rule &r) throw()
            {
                assert(rules.owns(&r));
                rules.move_to_front(&r);
            }

        }
    }
}

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            void grammar:: graphviz(const string &filename) const
            {
                ios::wcstream fp(filename);
                fp << "digraph G {\n";
                for(const rule *r=rules.head;r;r=r->next)
                {
                    r->viz(fp);
                }
                fp << "}\n";
            }
        }
    }
}
