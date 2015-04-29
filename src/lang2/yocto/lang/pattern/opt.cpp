#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {
        optional:: ~optional() throw() {}

        optional:: optional(pattern *p) throw() : joker( optional::UUID, p) {}


        pattern *optional:: create(pattern *p)
        {
            try
            {
                return new optional(p);
            }
            catch(...)
            {
                delete p;
                throw;
            }
        }

        pattern *optional:: clone() const
        {
            return create( jk->clone() );
        }

        bool optional:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {
            assert(jk);
            (void) (jk->match(tkn, src, fp));
            return true;
        }

        bool optional:: accept_empty() const throw()
        {
            return true;
        }


        void optional:: viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);
            fp("[shape=egg,label=\"?\"];\n");
            jk->viz(fp);
            vizlink(fp,this, jk);
        }
    }
}
