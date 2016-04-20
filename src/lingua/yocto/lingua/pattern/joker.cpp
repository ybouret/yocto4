#include "yocto/lingua/pattern/joker.hpp"

namespace yocto
{
    namespace lingua
    {
        joker:: ~joker() throw()
        {
            assert(motif);
            delete motif;
            motif = 0;
        }

        joker:: joker(const uint32_t t, pattern *p) throw() :
        pattern(t),
        motif(p)
        {

        }

        joker:: joker(const joker &other) :
        pattern(other),
        motif(other.motif->clone())
        {
        }

        
    }
}

namespace yocto
{
    namespace lingua
    {

        optional:: ~optional() throw()
        {
        }

        optional:: optional(pattern *p) throw() : joker( UUID, p)
        {
            YOCTO_LINGUA_PATTERN_IS(optional);
        }

        optional:: optional(const optional &other) : joker(other)
        {
            YOCTO_LINGUA_PATTERN_IS(optional);
        }

        pattern *optional:: clone() const
        {
            return new optional(*this);
        }

        optional * optional:: create( pattern *p )
        {
            assert(p);
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

        void optional:: __viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp << "[label=\"?\",shape=egg];\n";
            motif->__viz(fp);
            fp.viz( (const pattern *)this ); fp << "->"; fp.viz(motif); fp << ";\n";
        }

        bool optional:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            if(motif->match(tkn,src))
            {
                return true;
            }
            else
            {
                return true;
            }
        }

        void optional:: __sav(ios::ostream &fp) const
        {
            motif->save(fp);
        }


        bool optional:: match_empty() const
        {
            return true;
        }

    }

}


namespace yocto
{
    namespace lingua
    {

        at_least:: ~at_least() throw()
        {
        }

        at_least:: at_least(pattern *p,const size_t n) throw() :
        joker(UUID, p),
        nmin(n)
        {
            YOCTO_LINGUA_PATTERN_IS(at_least);
        }

        at_least:: at_least(const at_least &other) :
        joker(other),
        nmin(other.nmin)
        {
            YOCTO_LINGUA_PATTERN_IS(at_least);
        }

        pattern *at_least:: clone() const
        {
            return new at_least(*this);
        }

        at_least * at_least:: create( pattern *p, const size_t n)
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

        void at_least:: __viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp("[label=\">=%u\",shape=egg];\n",unsigned(nmin));
            motif->__viz(fp);
            fp.viz( (const pattern *)this ); fp << "->"; fp.viz(motif); fp << ";\n";
        }

        void at_least:: __sav(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(nmin);
            motif->save(fp);
        }
        
        bool at_least:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            size_t count = 0;
            while(true)
            {
                token sub;
                if(!motif->match(sub,src))
                {
                    assert(0==sub.size);
                    break;
                }
                ++count;
                tkn.merge_back(sub);
            }
            if(count>=nmin)
            {
                return true;
            }
            else
            {
                assert(0==tkn.size);
                src.unread(tkn);
                return false;
            }
        }


        bool at_least:: match_empty() const
        {
            if(nmin<=0)
            {
                return true;
            }
            else
            {
                return motif->match_empty();
            }
        }

        pattern *zero_or_more(pattern *p)
        {
            return at_least::create(p,0);
        }

        pattern *one_or_more(pattern *p)
        {
            return at_least::create(p,1);
        }

    }

}


#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace lingua
    {

        counting:: ~counting() throw()
        {
        }

        counting:: counting(pattern *p,const size_t n,const size_t m) throw() :
        joker(UUID, p),
        nmin(min_of(n,m)),
        nmax(max_of(n,m))
        {
            YOCTO_LINGUA_PATTERN_IS(counting);
        }

        counting:: counting(const counting &other) :
        joker(other),
        nmin(other.nmin),
        nmax(other.nmax)
        {
            YOCTO_LINGUA_PATTERN_IS(counting);
        }

        pattern *counting:: clone() const
        {
            return new counting(*this);
        }

        counting * counting:: create( pattern *p, const size_t n, const size_t m)
        {
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

        void counting:: __viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp("[label=\"[%u:%u]\",shape=egg];\n",unsigned(nmin),unsigned(nmax));
            motif->__viz(fp);
            fp.viz( (const pattern *)this ); fp << "->"; fp.viz(motif); fp << ";\n";
        }

        void counting:: __sav(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(nmin);
            fp.emit<uint32_t>(nmax);
            motif->save(fp);
        }

        bool counting:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            size_t count = 0;
            while(true)
            {
                token sub;
                if(!motif->match(sub,src))
                {
                    assert(0==sub.size);
                    break;
                }
                ++count;
                tkn.merge_back(sub);
            }
            if(nmin<=count&&count<=nmax)
            {
                return true;
            }
            else
            {
                assert(0==tkn.size);
                return false;
            }
        }


        bool counting:: match_empty() const
        {
            if(nmin<=0)
            {
                return true;
            }
            else
            {
                return motif->match_empty();
            }
        }

    }
    
}



