#ifndef YOCTO_MATH_ALG_ELLIPSE_INCLUDED
#define YOCTO_MATH_ALG_ELLIPSE_INCLUDED 1

#include "yocto/math/v2d.hpp"
#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class ellipse
        {
        public:
            explicit ellipse();
            virtual ~ellipse() throw();
            
            void reset() throw();
            
            void append( T x, T y ) throw();
            
            inline const matrix<T> &__S() const throw() { return S; }
            inline const matrix<T> &__C() const throw() { return C; }
            
        private:
            matrix<T> S;
            matrix<T> C;
            YOCTO_DISABLE_COPY_AND_ASSIGN(ellipse);
        };
        
    }
}

#endif
