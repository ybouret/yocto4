#ifndef YOCTO_MK_SPARSE_ALGEBRA_INCLUDED
#define YOCTO_MK_SPARSE_ALGEBRA_INCLUDED 1

#include "yocto/math/sparse/matrix.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        struct sp_algebra
        {
            typedef sp_item<T> item_type;
            static void mul(    array<T> &v, const sp_matrix<T> &M, const array<T> &u ) throw();
			static void muladd( array<T> &v, const sp_matrix<T> &M, const array<T> &u ) throw();
			static void mulsub( array<T> &v, const sp_matrix<T> &M, const array<T> &u ) throw();
            
            static void mul_trn(    array<T> &v, const sp_matrix<T> &M, const array<T> &u ) throw();
            static void muladd_trn( array<T> &v, const sp_matrix<T> &M, const array<T> &u ) throw();
			static void mulsub_trn( array<T> &v, const sp_matrix<T> &M, const array<T> &u ) throw();


        };
        
    }
}

#endif
