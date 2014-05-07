#include "yocto/math/ztype.hpp"
#include "yocto/math/sparse/matrix.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

namespace yocto
{
    
    namespace math
    {
        template <>
        spmatrix<z_type>:: ~spmatrix() throw()
        {
        }
        
        
        template <>
        spmatrix<z_type>:: spmatrix() throw() :
        rows(0),
        cols(0),
        nmax(0),
        items()
        {
            
        }
        
        template <>
        spmatrix<z_type>:: spmatrix(size_t nr,size_t nc) :
        rows(nr),
        cols(nc),
        nmax(rows*cols),
        items()
        {
            if ( (nr<=0&&nc>0) || (nr>0&&nc<=0) )
                throw libc::exception(EINVAL,"invalid sparse matrix dimension");
        }
        
        template <>
        z_type & spmatrix<z_type>:: operator()(size_t i,size_t j)
        {
            assert(i>0);
            assert(j>0);
            assert(i<=rows);
            assert(j<=cols);
            const sp_key key(i,j);
            item_type *p = items.search(key);
            if(!p)
            {
                const item_type it(i,j,numeric<z_type>::zero);
                if(!items.insert(it))
                    throw exception("unexpected sparse matrix insertion failure");
                p = items.search(key);
                assert(p);
            }
            return p->value;
        }
        
        template <>
        z_type spmatrix<z_type>:: operator()(size_t i, size_t j) const throw()
        {
            assert(i>0);
            assert(j>0);
            assert(i<=rows);
            assert(j<=cols);
            const sp_key key(i,j);
            const item_type *p = items.search(key);
            return p ? p->value : numeric<z_type>::zero;
        }
        
        
        
        template <>
        void  spmatrix<z_type>:: output( std::ostream &os ) const
        {
            const spmatrix<z_type> &self = *this;
            os << "[";
            for(size_t i=1;i<=rows;++i)
            {
                for(size_t j=1;j<=cols;++j)
                {
                    os << ' ' << self(i,j);
                }
                if(i<rows) os << ';';
            }
            os << "]";
        }
        
        template <>
        void spmatrix<z_type>:: ensure(size_t num_items)
        {
            if(num_items>nmax) num_items = nmax;
            if(num_items>items.capacity())
                items.reserve(num_items-items.capacity());
        }

        
        template <>
        void spmatrix<z_type>:: ldz() throw()
        {
            items.free();
        }

        
    }
    
}