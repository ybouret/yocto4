#ifndef YOCTO_MATH_ALG_SHAPES_INCLUDED
#define YOCTO_MATH_ALG_SHAPES_INCLUDED 1

#include "yocto/math/v2d.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class fit_shape
        {
        public:
            virtual ~fit_shape() throw();
            
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
            
            inline const matrix<T> & __S() const throw() { return S; }
            inline const array<T>  & __Q() const throw() { return Q; }
            
            bool solve( T &R, v2d<T> &C ) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(fit_circle);
            matrix<T> S;
            vector<T> Q;
            lu<T>     LU;
            
        };
        
        
#if 0
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
#endif
        
    }
}

#endif
