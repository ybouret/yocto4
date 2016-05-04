#ifndef YOCTO_MATH_ALG_SHAPES_INCLUDED
#define YOCTO_MATH_ALG_SHAPES_INCLUDED 1

#include "yocto/math/point2d.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/container/matrix.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class fit_shape
        {
        public:
            virtual ~fit_shape() throw();
            
            virtual void reset() throw() = 0;
            virtual void append( T x, T y ) throw() = 0;
            
        protected:
            explicit fit_shape() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(fit_shape);
            
        };
        
        template <typename T>
        class fit_circle : public fit_shape<T>
        {
        public:
            explicit fit_circle();
            virtual ~fit_circle() throw();
            
            virtual void reset() throw();
            virtual void append(T x, T y) throw();
                        
            //! throw upon failure
            void solve( T &R, point2d<T> &C ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(fit_circle);
            matrix<T>         S;
            vector<T>         Q;
        };
        
        enum conic_type
        {
            conic_generic,
            conic_ellipse
        };
        
        template <typename T>
        class fit_conic : public fit_shape<T>
        {
        public:
            explicit fit_conic();
            virtual ~fit_conic() throw();
            
            virtual void reset() throw();
            virtual void append( T x, T y ) throw();
            
            
            void solve(conic_type t,array<T> &param) const;
            
            static void reduce(point2d<T>   &center,
                               point2d<T>   &radius,
                               matrix<T>    &rotation,
                               array<T>     &param );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(fit_conic);
            matrix<T> Sqq;
            matrix<T> Sqz;
            matrix<T> Szz;
            
        };
        
        
        
    }
}

#endif
