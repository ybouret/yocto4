#ifndef YOCTO_MATH_ALG_SHAPES_INCLUDED
#define YOCTO_MATH_ALG_SHAPES_INCLUDED 1

#include "yocto/math/v2d.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/matrix.hpp"

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
            vector<T> W; // for SVD build
            matrix<T> V; // for SVD build
            vector<T> A; // for SVD solve
        };
        
        
        template <typename T>
        class fit_conic : public fit_shape<T>
        {
        public:
            explicit fit_conic();
            virtual ~fit_conic() throw();
            
            void reset() throw();
            
            void append( T x, T y ) throw();
            
            
            bool solve();
            
        private:
            matrix<T> Sqq;
            matrix<T> Sqz;
            matrix<T> Szz;
            matrix<T> C;
            YOCTO_DISABLE_COPY_AND_ASSIGN(fit_conic);
        };
        
    }
}

#endif
