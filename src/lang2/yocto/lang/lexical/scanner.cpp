#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {

            scanner:: ~scanner() throw() {}

            scanner:: scanner(const string &id , int &ir) :
            name(id),
            iline(ir),
            rules(),
            cache()
            {
            }

            void scanner:: check_label(const string &label)
            {
                for(const rule *r=rules.head;r;r=r->next)
                {
                    if(r->label==label)
                    {
                        throw exception("multiple rule '%s' in <%s>", label.c_str(), name.c_str());
                    }
                }
            }

        }
    }

}

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {
            void  scanner:: make(const string &label, pattern *p, const action &a)
            {
                auto_ptr<pattern> q(p);
                check_label(label);
                rules.push_back( rule::create(label,q.yield(),a) );
            }

            void  scanner:: make(const char *l, const char *expr, const action &a, const p_dict *dict)
            {
                const string label(l);
                make(label, regexp(expr,dict), a);
            }
        }

    }
}
