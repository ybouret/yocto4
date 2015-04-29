#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {
        counting:: ~counting() throw() {}

        counting:: counting(pattern *p, size_t n, size_t m) throw() :
        joker(UUID, p),
        lower(n),
        upper(m)
        {
            assert(n<=m);
        }


        pattern *counting:: create(pattern *p, size_t n, size_t m)
        {
            if(n>m) cswap(n,m);
            assert(p);
            try
            {
                return new counting(p,n,m);
            }
            catch(...)
            {
                delete p;
                throw;
            }

        }

        pattern *counting:: clone() const
        {
            return create( jk->clone(), lower, upper);
        }

        bool counting:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {
            token  stk;
            size_t num = 0;

            while(jk->match(stk,src,fp))
            {
                ++num;
            }

            if(num>=lower && num<=upper)
            {
                tkn.merge_back(stk);
                return true;
            }
            else
            {
                src.unget(stk);
                return false;
            }
        }

        bool counting:: accept_empty() const throw()
        {
            return lower<=0 || jk->accept_empty();
        }
        
        void counting:: viz( ios::ostream &fp ) const
        {
            fp.viz((const pattern *)this);
            fp("[shape=egg,label=\"[%u,%u]\"];\n", unsigned(lower), unsigned(upper) );
            jk->viz(fp);
            vizlink(fp,this, jk);
        }

    }
}
