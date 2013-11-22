#ifndef YOCTO_MATH_EXTEND_INCLUDED
#define YOCTO_MATH_EXTEND_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
    namespace math
    {
        
        enum extend_mode
        {
            extend_constant,
            extend_cyclic
        };
        
        template <typename T>
        class extend
        {
        public:
            explicit extend( extend_mode lo, extend_mode up) throw();
            virtual ~extend() throw();
            
            extend_mode lower;
            extend_mode upper;
        
            T operator()( T x0, const array<T> &X, const array<T> &Y ) const throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(extend);
        };
        
    }
}

#endif
