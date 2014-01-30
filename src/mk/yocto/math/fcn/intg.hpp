#ifndef YOCTO_MATH_FCN_INTG_INCLUDED
#define YOCTO_MATH_FCN_INTG_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/core/pool.hpp"

namespace yocto
{
    
    namespace math
    {
        
        template <typename T>
        class integrator
        {
        public:
            class range
            {
            public:
                const T a,b,eps;
                range *next;
                range(T,T,T) throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(range);
                ~range() throw();
            };
            
            class rpool : public core::pool_of<range>
            {
            public:
                explicit rpool() throw();
                virtual ~rpool() throw();
                void     cleanup() throw();
                void     pour_into( rpool &collector ) throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rpool);
            };
            
            
            explicit integrator() throw();
            virtual ~integrator() throw();
            
            range *create(T,T,T);
            void   store(range *r) throw();
            
            
            T operator()( T a, T b, typename numeric<T>::function &F, T ftol );
            
            
            static bool quad( T &ans, T a, T b, typename numeric<T>::function &F, T ftol);
            
            rpool pool;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(integrator);
        };
        
    }
    
}

#endif

