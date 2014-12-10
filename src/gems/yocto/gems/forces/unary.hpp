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
            virtual ~unary_force() throw() {}
            
            virtual T operator()( atom<T> &p ) const = 0;
            
        protected:
            explicit unary_force() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(unary_force);
        };
        
    }
}

#endif
