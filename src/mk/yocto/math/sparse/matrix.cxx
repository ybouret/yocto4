#include "yocto/math/ztype.hpp"
#include "yocto/math/sparse/matrix.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace math
    {
        template <>
        spmatrix<z_type>:: ~spmatrix() throw()
        {
            if(size>0)
            {
                assert(na>0);
                ++sa;
                memory::kind<memory::global>::release_as<z_type>(sa,na);
                (size_t&)size = 0;
            }
        }
        
        template <>
        void spmatrix<z_type>::build()
        {
            if(size>0)
            {
                na = size;
                sa = memory::kind<memory::global>::acquire_as<z_type>(na)-1;
            }
            
        }
        
        template <>
        spmatrix<z_type>:: spmatrix() throw() :
        size(0),
        sa(0),
        na(0),
        items()
        {
            
        }
        
        template <>
        spmatrix<z_type>:: spmatrix(size_t n) :
        size(n),
        sa(0),
        na(0),
        items()
        {
            build();
        }
        
        template <>
        z_type & spmatrix<z_type>:: operator()(size_t i,size_t j)
        {
            assert(i>0);
            assert(j>0);
            assert(i<=size);
            assert(j<=size);
            if(i!=j)
            {
                const sp_key k(i,j);
                item_type   *p = items.search(k);
                if(!p)
                {
                    {
                        const item_type it(i,j,numeric<z_type>::zero);
                        if(!items.insert(it))
                            throw exception("unexpected sparse item insertion failure @(%u,%u)", unsigned(i), unsigned(j));
                    }
                    p = items.search(k);
                    assert(0!=p);
                }
                return p->value;
            }
            else
            {
                return sa[i];
            }
        }
        
        template <>
        z_type spmatrix<z_type>:: operator()(size_t i, size_t j) const throw()
        {
            assert(i>0);
            assert(j>0);
            assert(i<=size);
            assert(j<=size);
            if(i!=j)
            {
                const sp_key     k(i,j);
                const item_type *p = items.search(k);
                return p ? p->value : numeric<z_type>::zero;
            }
            else
            {
                return sa[i];
            }
        }
        
        template <>
        size_t spmatrix<z_type>::extras() const throw()
        {
            return items.size();
        }

        template <>
        void  spmatrix<z_type>:: output( std::ostream &os ) const
        {
            const spmatrix<z_type> &self = *this;
            os << "[";
            for(size_t i=1;i<=size;++i)
            {
                for(size_t j=1;j<=size;++j)
                {
                    os << ' ' << self(i,j);
                }
                if(i<size) os << ';';
            }
            os << "]";
        }
    }
    
}