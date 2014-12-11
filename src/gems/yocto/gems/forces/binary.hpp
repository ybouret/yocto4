#ifndef YOCTO_FORCES_BINARY_INCLUDED
#define YOCTO_FORCES_BINARY_INCLUDED 1

#include "yocto/gems/atom.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class binary_force : public counted_object
        {
        public:
            typedef arc_ptr<binary_force> pointer;

            virtual ~binary_force() throw() {}
            
            virtual T operator()( atom<T> &p, atom<T> &q ) const = 0;
            
        protected:
            explicit binary_force() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(binary_force);
        };
        
        template <typename T>
        class spring_force : public binary_force<T>
        {
        public:
            const T k;
            const T l;
            
            virtual T operator()( atom<T> &p, atom<T> &q ) const
            {
                const v3d<T> dr(p,q);
                const T      pq2   = dr.norm2();
                const T      pq    = Sqrt(pq2);
                const T      delta = pq - l;
                const T      k_del = k * delta;
                const v2d<T> f     = (k_del/pq) * dr;
                p.f += f;
                q.f -= f;
                return T(0.5)*k_del * delta;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spring_force);
        };
        
    }
}

#endif
