#include "yocto/math/ztype.hpp"
#include "yocto/math/sparse/algebra.hpp"

namespace yocto
{
    namespace math
    {
        template <>
        void sp_algebra<z_type>::mul(array<z_type> &v, const sp_matrix<z_type> &M, const array<z_type> &u) throw()
        {
            static const z_type __zero__ = numeric<z_type>::zero;
            assert(v.size()==M.rows);
            assert(u.size()==M.cols);
            for(size_t r=v.size();r>0;--r)
            {
                v[r] = __zero__;
            }

            for(size_t i=M.count();i>0;--i)
            {
                const item_type &it = M[i];
                v[it.row] += it.value * u[it.col];
            }
        }
     
        template <>
        void sp_algebra<z_type>::muladd(array<z_type> &v, const sp_matrix<z_type> &M, const array<z_type> &u) throw()
        {
            assert(v.size()==M.rows);
            assert(u.size()==M.cols);
            
            
            for(size_t i=M.count();i>0;--i)
            {
                const item_type &it = M[i];
                v[it.row] += it.value * u[it.col];
            }
        }

        template <>
        void sp_algebra<z_type>::mulsub(array<z_type> &v, const sp_matrix<z_type> &M, const array<z_type> &u) throw()
        {
            assert(v.size()==M.rows);
            assert(u.size()==M.cols);
            
            
            for(size_t i=M.count();i>0;--i)
            {
                const item_type &it = M[i];
                v[it.row] -= it.value * u[it.col];
            }
        }
        
        
    }
    
}
