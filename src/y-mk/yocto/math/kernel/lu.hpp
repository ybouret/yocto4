#ifndef YOCTO_MATH_KERNEL_LU_INCLUDED
#define YOCTO_MATH_KERNEL_LU_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        
              
        template <typename T>
        struct lu
        {
            
            typedef typename real_of<T>::type real_type;
            
            
            static bool build(matrix<T>        &a,
                              array<size_t>    &indx,
                              array<real_type> &scal,
                              bool             *dneg_p=0) throw();
            
            static void solve(const matrix<T>     &a,
                              const array<size_t> &indx,
                              array<T>            &b) throw();
            
            static void solve(const matrix<T>     &M,
                              const array<size_t> &indx,
                              matrix<T>           &Q) throw();
        };
        
    }
    
}

#endif

