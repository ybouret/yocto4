#include "yocto/lang/regexp/compiler.hxx"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {

        RegExp:: ~RegExp() throw() {}

        RegExp:: RegExp(const char *expr, const size_t n, const p_dict *d) :
        curr(expr),
        last(expr+n),
        dict(d),
        level(0)
        {
            assert(expr!=NULL);
        }

    }
}

namespace yocto
{
    namespace lang
    {

        logical *RegExp:: parse_expr()
        {

            auto_ptr<logical> p(AND::create());
            const char *ini = curr;

            while(curr<last)
            {
                char C = curr[0];
                switch(C)
                {
                    default:
                        break;
                }
                p->append( single::create(C) );
                ++curr;
            }

            if(p->operands.size<=0)
            {
                throw exception("empty sub-expression from '%s'", ini);
            }

            return p.yield();
        }

    }

}
