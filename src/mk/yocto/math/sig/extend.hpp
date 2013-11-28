#ifndef YOCTO_MATH_EXTEND_INCLUDED
#define YOCTO_MATH_EXTEND_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/math/v2d.hpp"

namespace yocto
{
    namespace math
    {
        
        enum extend_mode
        {
            extend_constant,   //!< send extremal value
            extend_cyclic,     //!< assume cyclic value
            extend_odd,        //!< assume oddity/boundary
            extend_even        //!< assume even/boundary
        };
        
        enum extend_value
        {
            extend_eval, //!< get filtered value
            extend_diff  //!< get fileterd derivative
        };
        
        template <typename T>
        class extend
        {
        public:
            explicit extend( extend_mode lo, extend_mode up) throw();
            explicit extend( extend_mode both ) throw();
            
            virtual ~extend() throw();
            
            extend_mode lower;
            extend_mode upper;
            
            v2d<T> at( ptrdiff_t i, const array<T> &X, const array<T> &Y ) const;
            
            
            void operator()(extend_value    kind,
                            array<T>       &Z,
                            const array<T> &X,
                            const array<T> &Y,
                            const T         dt_prev,
                            const T         dt_next,
                            const size_t    degree
                            ) const;
            
            inline void eval(array<T>       &Z,
                             const array<T> &X,
                             const array<T> &Y,
                             const T         dt,
                             const size_t    degree
                             ) const
            {
                const T half = dt/2;
                (*this)(extend_eval,Z,X,Y,half,half,degree);
            }
            
            inline void diff(array<T>       &Z,
                             const array<T> &X,
                             const array<T> &Y,
                             const T         dt,
                             const size_t    degree
                             ) const
            {
                const T half = dt/2;
                (*this)(extend_diff,Z,X,Y,half,half,degree);
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(extend);
            T get_x(ptrdiff_t i, const array<T> &X, const ptrdiff_t N, const T L) const throw();
            T get_y(ptrdiff_t i, const array<T> &Y, const ptrdiff_t N) const throw();
        };
        
    }
}

#endif
