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

        void joker:: __out(ios::ostream &fp) const
        {
            motif->save(fp);
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

    }

}


namespace yocto
{
    namespace lingua
    {

        zero_or_more:: ~zero_or_more() throw()
        {
        }

        zero_or_more:: zero_or_more(pattern *p) throw() : joker(UUID, p)
        {
            YOCTO_LINGUA_PATTERN_IS(zero_or_more);
        }

        zero_or_more:: zero_or_more(const zero_or_more &other) : joker(other)
        {
            YOCTO_LINGUA_PATTERN_IS(zero_or_more);
        }

        pattern *zero_or_more:: clone() const
        {
            return new zero_or_more(*this);
        }

        zero_or_more * zero_or_more:: create( pattern *p )
        {
            assert(p);
            try
            {
                return new zero_or_more(p);
            }
            catch(...)
            {
                delete p;
                throw;
            }
        }

        void zero_or_more:: __viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp << "[label=\">=0\",shape=egg];\n";
            motif->__viz(fp);
            fp.viz( (const pattern *)this ); fp << "->"; fp.viz(motif); fp << ";\n";
        }

        bool zero_or_more:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            while(true)
            {
                token sub;
                if(!motif->match(sub,src))
                {
                    assert(0==sub.size);
                    break;
                }
                tkn.merge_back(sub);
            }
            return true;
        }

    }

}

namespace yocto
{
    namespace lingua
    {

        one_or_more:: ~one_or_more() throw()
        {
        }

        one_or_more:: one_or_more(pattern *p) throw() : joker(UUID, p)
        {
            YOCTO_LINGUA_PATTERN_IS(one_or_more);
        }

        one_or_more:: one_or_more(const one_or_more &other) : joker(other)
        {
            YOCTO_LINGUA_PATTERN_IS(one_or_more);
        }

        pattern *one_or_more:: clone() const
        {
            return new one_or_more(*this);
        }

        one_or_more * one_or_more:: create( pattern *p )
        {
            assert(p);
            try
            {
                return new one_or_more(p);
            }
            catch(...)
            {
                delete p;
                throw;
            }
        }

        void one_or_more:: __viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp << "[label=\">=1\",shape=egg];\n";
            motif->__viz(fp);
            fp.viz( (const pattern *)this ); fp << "->"; fp.viz(motif); fp << ";\n";
        }

        bool one_or_more:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
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
            if(count>0)
            {
                return true;
            }
            else
            {
                assert(0==tkn.size);
                return false;
            }
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

        void counting:: __out(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(nmin);
            fp.emit<uint32_t>(nmax);
            joker::__out(fp);
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
        
    }
    
}



