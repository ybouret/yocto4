#ifndef YOCTO_MK_SIG_EXTENDER_INCLUDED
#define YOCTO_MK_SIG_EXTENDER_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
    namespace math
    {
        
        enum extension_type
        {
            extend_zero,     //!< returns 0 out of boundaries
            extend_constant, //!< use the boundary value
            extend_cyclic,   //!< assume the data are cyclic
            extend_odd,      //!< odd/boundary
            extend_even      //!< even/boundary
        };
        
        
        template <typename T>
        class extender
        {
        public:
            explicit extender( extension_type lo, extension_type up);
            explicit extender( extension_type both) throw();
            virtual ~extender() throw();

            const extension_type lower;
            const extension_type upper;
            
            T get_x(ptrdiff_t i, const array<T> &X, const ptrdiff_t N) const throw();
            T get_y(ptrdiff_t i, const array<T> &Y, const ptrdiff_t N) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(extender);
            

        };
    
        
    }
}


#endif

