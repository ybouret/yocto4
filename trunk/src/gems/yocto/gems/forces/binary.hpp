#ifndef YOCTO_FORCES_UNARY_INCLUDED
#define YOCTO_FORCES_UNARY_INCLUDED 1

#include "yocto/gems/atom.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class binary_force : public counted_object
        {
        public:
            virtual ~binary_force() throw() {}
            
            virtual T operator()( atom<T> &p, atom<T> &q ) const = 0;
            
        protected:
            explicit binary_force() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(binary_force);
        };
        
    }
}

#endif
