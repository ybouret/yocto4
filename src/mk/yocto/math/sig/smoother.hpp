#ifndef YOCTO_MK_SIG_SMOOTHER_INCLUDED
#define YOCTO_MK_SIG_SMOOTHER_INCLUDED 1

#include "yocto/math/sig/extender.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/math/v2d.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class smoother
        {
        public:
            typedef v2d<T> vtx_t;
            
            explicit smoother() throw();
            virtual ~smoother() throw();
            
            T           lower_range;
            T           upper_range;
            size_t      degree;
            
            list<vtx_t> v;
            
            T get(size_t             i,
                  const array<T>    &X,
                  const array<T>    &Y,
                  const extender<T> &E,
                  T                 *dYdX);
            
            //! single pass
            void run(array<T>          &Z,
                     const array<T>    &X,
                     const array<T>    &Y,
                     const extender<T> &E,
                     array<T>          *dZdX);
            
            //! double pass
            void full(array<T>          &Z,
                      const array<T>    &X,
                      const array<T>    &Y,
                      const extender<T> &E,
                      array<T>          &dZdX);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(smoother);
        };
        
    }
}

#endif


