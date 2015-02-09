#ifndef YOCTO_FORCES_UNARY_INCLUDED
#define YOCTO_FORCES_UNARY_INCLUDED 1

#include "yocto/gems/atom.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class unary_force : public counted_object
        {
        public:
            typedef arc_ptr<unary_force> pointer;
            
            virtual ~unary_force() throw() {}
            
            virtual T update( atom<T> &p ) const = 0;
            
        protected:
            explicit unary_force() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(unary_force);
        };
        
        template <typename T>
        class gravity_force : public unary_force<T>
        {
        public:
            const v3d<T> g;
            explicit gravity_force( const v3d<T> value) throw() :
            g(value)
            {}
            
            virtual ~gravity_force() throw() {}
            
            virtual T update( atom<T> &p ) const
            {
                const v3d<T> f = p.m * g;
                p.f += f;
                return - f*p.r;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(gravity_force);
        };
        
        template <typename T>
        class friction_force : public unary_force<T>
        {
        public:
            const T lambda;
            explicit friction_force(const double value) throw() :
            lambda(value)
            {
            }
            
            virtual ~friction_force() throw()
            {
            }
           
            virtual T update( atom<T> &p ) const
            {
                p.f -= lambda * p.v;
                return 0;
            }

            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(friction_force);
        };
        
    }
}

#endif
