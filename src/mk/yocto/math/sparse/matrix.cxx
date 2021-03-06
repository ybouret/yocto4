#include "yocto/math/ztype.hpp"
#include "yocto/math/sparse/matrix.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/sequence/vector.hpp"

#include <cerrno>

namespace yocto
{
    
    namespace math
    {
        template <>
        sp_matrix<z_type>:: ~sp_matrix() throw()
        {
        }
        
        
        template <>
        sp_matrix<z_type>:: sp_matrix() throw() :
        rows(0),
        cols(0),
        nmax(0),
        items()
        {
            
        }
        
        template <>
        sp_matrix<z_type>:: sp_matrix(size_t nr,size_t nc) :
        rows(nr),
        cols(nc),
        nmax(rows*cols),
        items()
        {
            if ( (nr<=0&&nc>0) || (nr>0&&nc<=0) )
                throw libc::exception(EINVAL,"invalid sparse matrix dimension");
        }
        
        template <>
        z_type & sp_matrix<z_type>:: operator()(size_t i,size_t j)
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
        z_type sp_matrix<z_type>:: operator()(size_t i, size_t j) const throw()
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
        void  sp_matrix<z_type>:: output( std::ostream &os ) const
        {
            const sp_matrix<z_type> &self = *this;
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
        void sp_matrix<z_type>:: ensure(size_t n)
        {
            if(n >nmax) n = nmax;
            if(n >items.capacity())
                items.reserve(n-items.capacity());
        }
        
        
        template <>
        void sp_matrix<z_type>:: ldz() throw()
        {
            items.free();
        }
        
        
        template <>
        void sp_matrix<z_type>::cleanup( real_type threshold ) throw()
        {
            vector<sp_key> keys;
            // collect keys
            for(size_t i=items.size();i>0;--i)
            {
                const item_type &it = items[i];
                if( Fabs(it.value) <= threshold)
                {
                    keys.push_back(it);
                }
            }
            // remove keys
            for(size_t i=keys.size();i>0;--i)
            {
                (void) items.remove( keys[i] );
            }
        }
        
        
        template <>
        sp_matrix<z_type>:: sp_matrix(const sp_matrix &M) :
        rows(M.rows),
        cols(M.cols),
        nmax(M.nmax),
        items(M.items)
        {
            
        }
        
        template <>
        sp_matrix<z_type>:: sp_matrix( const matrix<z_type> &M, real_type threshold ) :
        rows(M.rows),
        cols(M.cols),
        nmax(M.items),
        items()
        {
            // first pass: count non zero
            size_t nz = 0;
            for(size_t i=M.rows;i>0;--i)
            {
                for(size_t j=M.cols;j>0;--j)
                {
                    if( Fabs(M[i][j]) > threshold ) ++nz;
                }
            }
            items.reserve(nz);
            
            
            // second pass: register
            for(size_t i=1;i<=M.rows;++i)
            {
                for(size_t j=1;j<=M.cols;++j)
                {
                    const z_type &m = M[i][j];
                    if( Fabs(m) > threshold )
                    {
                        const item_type it(i,j,m);
                        if( !items.insert(it))
                            throw exception("unexpected sparse matrix constructor failure");
                    }
                }
            }
            
        }
        
        namespace
        {
            static inline void __transpose__( sp_key &k ) throw()
            {
                cswap_const(k.row,k.col);
            }
        }
        
        template <>
        void sp_matrix<z_type>:: transpose() throw()
        {
            items.alter_keys(__transpose__);
            cswap_const(rows,cols);
        }

        
    }
    
}
