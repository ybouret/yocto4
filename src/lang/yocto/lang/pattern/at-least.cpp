#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {
        at_least:: ~at_least() throw() {}
        
        at_least:: at_least(pattern *p, size_t n) throw() :
        joker(UUID, p),
        count(n)
        {}
        
        pattern *at_least:: create(pattern *p, size_t n)
        {
            assert(p);
            try
            {
                return new at_least(p,n);
            }
            catch(...)
            {
                delete p;
                throw;
            }
            
        }
        
        
        pattern *at_least:: clone() const
        {
            return create( jk->clone(), count);
        }
        
        bool at_least:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {
            token stk;
            size_t num = 0;
            while(jk->match(stk,src,fp))
            {
                ++num;
            }
            if(num>=count)
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

        bool at_least:: accept_empty() const throw()
        {
            return (count<=0) || jk->accept_empty();
        }
        
        void at_least:: viz( ios::ostream &fp ) const
        {
            fp.viz((const pattern *)this);
            fp("[shape=egg,label=\">=%u\"];\n", unsigned(count) );
            jk->viz(fp);
            vizlink(fp,this, jk);
        }

        void at_least:: save( ios::ostream &fp ) const
        {
            fp.emit(uuid);
            fp.emit<uint32_t>(count);
            jk->save(fp);
        }

    }
}
