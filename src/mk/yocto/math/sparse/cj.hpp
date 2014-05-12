#ifndef YOCTO_MK_SPARSE_CJ_INCLUDED
#define YOCTO_MK_SPARSE_CJ_INCLUDED 1

#include "yocto/math/sparse/matrix.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {
        template <typename T>
        class sp_cgrad
        {
        public:
            explicit sp_cgrad() throw();
            explicit sp_cgrad(size_t n); //!< reserve some memory
            virtual ~sp_cgrad() throw();
            
            void solve( array<T> &p, const sp_matrix<T> &A, const array<T> &b );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(sp_cgrad);
            vector<T> g,h,xi,z,q,r;
        };
        
    }
}

#endif
