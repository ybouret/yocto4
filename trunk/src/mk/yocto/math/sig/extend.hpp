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
            extend_zero,        //!< send zero
            extend_constant,   //!< send extremal value
            extend_cyclic,     //!< assume cyclic value
            extend_odd,        //!< assume oddity/boundary
            extend_even        //!< assume even/boundary
        };
        
        extend_mode extend_mode_for_derivative( extend_mode m ) throw();
        
        
        template <typename T>
        class extend
        {
        public:
            //! TODO: bug with cyclic conditions !!!! => same values in local array...
            
            explicit extend( extend_mode lo, extend_mode up) throw();
            explicit extend( extend_mode both ) throw();
            
            virtual ~extend() throw();
            
            extend_mode lower;
            extend_mode upper;
            
            v2d<T> at( ptrdiff_t i, const array<T> &X, const array<T> &Y ) const;
            
            
            ///! asymetric operator
            T operator()(array<T>       &Z,
                         const array<T> &X,
                         const array<T> &Y,
                         const T         dt_prev,
                         const T         dt_next,
                         const size_t    degree,
                         array<T>       *dZdX
                         ) const;
            
            
            //! centered operator
            inline T operator()(array<T>       &Z,
                                const array<T> &X,
                                const array<T> &Y,
                                const T         dt,
                                const size_t    degree,
                                array<T>       *dZdX
                                ) const
            {
                const extend  &self = *this;
                const T  half = dt/2;
                return self(Z,X,Y,half,half,degree,dZdX);
            }
            
            
            void build_rms(sequence<T>    &dx,
                           sequence<T>    &rms,
                           const array<T> &X,
                           const array<T> &Y,
                           const size_t   degree) const;
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(extend);
            T get_x(ptrdiff_t i, const array<T> &X, const ptrdiff_t N) const throw();
            T get_y(ptrdiff_t i, const array<T> &Y, const ptrdiff_t N) const throw();
        };
        
        //! for accompanying derivatives
        template <typename T>
        class extend2
        {
        public:
            explicit extend2(extend_mode lo, extend_mode up) throw();
            explicit extend2(extend_mode both) throw();
            virtual ~extend2() throw();
            
            //! two pass filtering
            /**
             smooth function and evaluate derivative
             \return the RMS between Z (filtered) and Y
             */
            T operator()(array<T>       &Z,
                         const array<T> &X,
                         const array<T> &Y,
                         const T         dt_prev,
                         const T         dt_next,
                         const size_t    degree,
                         array<T>       &dZdX) const;
            
            //! symetric wrapper
            inline
            T operator()(array<T>       &Z,
                         const array<T> &X,
                         const array<T> &Y,
                         const T         dt,
                         const size_t    degree,
                         array<T>       &dZdX) const
            {
                const extend2  &self = *this;
                const T  half = dt/2;
                return self(Z,X,Y,half,half,degree,dZdX);

            }
            
        private:
            YOCTO_DISABLE_ASSIGN(extend2);
            extend<T> func;
            extend<T> diff;
        };
        
    }
}

#endif
