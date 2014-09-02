#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {
        joker:: ~joker() throw()
        {
            assert(motif);
            delete motif;
            motif = 0;
        }
        
        joker:: joker( const uint32_t id, pattern *p ) throw() :
        pattern(id),
        motif(p)
        {
            assert(motif);
        }
        
        void joker:: reset() throw()
        {
            assert(motif);
            motif->clear();
            this->clear();
        }
        
        void joker:: __viz( const void *parent, ios::ostream &fp ) const
        {
            motif->viz(fp);
            fp.viz(parent); fp << " -> "; fp.viz(motif); fp << ";\n";
        }
        
        
        void joker:: refactor() throw()
        {
            assert(motif);
            motif->refactor();
        }
    }
    
}


namespace yocto
{
    namespace lang
    {
        optional:: ~optional() throw()
        {
            
        }
        
        optional:: optional(pattern *p) throw() :
        joker(tag,p)
        {
            self = (optional *)this;
        }
        
        pattern * optional:: create( pattern * p )
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
            return create( motif->clone() );
        }
        
        bool optional:: match( source &src, ios::istream &fp)
        {
            assert(0==size);
            assert(motif);
            assert(0==motif->size);
            if(motif->match(src,fp))
            {
                this->merge_back(*motif);
            }
            return true;
        }
        
        void optional:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [label=\"?\"];\n";
            __viz(this, fp);
        }

        
    }
    
}

namespace yocto
{
    namespace lang
    {
        
        at_least:: ~at_least() throw()
        {
        }
        
        at_least:: at_least(pattern *p,size_t val) throw() :
        joker(tag,p),
        value(val)
        {
            self = (at_least *)this;
        }
        
        pattern * at_least:: clone() const
        {
            return create( motif->clone(), value );
        }
        
        
        pattern * at_least:: create( pattern *p, size_t n )
        {
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
        
        
        bool at_least:: match( source &src, ios::istream &fp)
        {
            assert(0==size);
            assert(motif);
            assert(0==motif->size);
            size_t count = 0;
            while(motif->match(src,fp))
            {
                merge_back(*motif);
                ++count;
            }
            assert(0==motif->size);
            if(count>=value)
            {
                return true;
            }
            else
            {
                src.unget(*this);
                assert(0==size);
                return false;
            }
        }
        
        void at_least:: viz( ios::ostream &fp) const
        {
            fp.viz(this);
            fp(" [label=\">=%u\"];\n", unsigned(value));
            __viz(this, fp);
        }

        
        pattern * zero_or_more(pattern *p)
        {
            return at_least::create(p,0);
        }
        
        pattern * one_or_more(pattern *p)
        {
            return at_least::create(p,1);
        }
        
    }
    
}

#include "yocto/code/utils.hpp"
namespace yocto
{
    namespace lang
    {
        
        counting:: ~counting() throw()
        {
            
        }
        
        
        counting:: counting( pattern *p, const size_t lo, const size_t up ) throw() :
        joker(tag,p),
        n( min_of(lo,up) ),
        m( max_of(lo,up) )
        {
            self = (counting *)this;
        }
        
        pattern *counting:: create( pattern *p, const size_t lo, const size_t up )
        {
            try
            {
                return new counting(p,lo,up);
            }
            catch(...)
            {
                delete p;
                throw;
            }
        }
        
        pattern * counting:: clone() const
        {
            return create(motif->clone(),n,m);
        }
        
        bool counting:: match(source &src, ios::istream &fp)
        {
            
            assert(0==size);
            assert(motif);
            assert(0==motif->size);
            size_t count = 0;
            
            // minimum count: n
            while(count<n && motif->match(src,fp))
            {
                merge_back(*motif);
                ++count;
            }
            if(count<n)
            {
                src.unget(*this);
                return false;
            }
            
            while(count<m&&motif->match(src,fp))
            {
                merge_back(*motif);
                ++count;
            }
            
            return true;
        }
        
        void counting:: viz( ios::ostream &fp) const
        {
            fp.viz(this);
            fp(" [label=\"{%u,%u}\"];\n", unsigned(n), unsigned(m));
            __viz(this, fp);
        }

    }
    
}



