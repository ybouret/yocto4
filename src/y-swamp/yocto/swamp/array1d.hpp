#ifndef YOCTO_SWAMP_ARRAY1D_INCLUDED
#define YOCTO_SWAMP_ARRAY1D_INCLUDED 1

#include "yocto/swamp/in1d.hpp"

namespace yocto 
{
    namespace swamp
    {
        
        
        template <typename T>
        class array1D : public linear<T,layout1D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef linear<T,layout1D> linear_type;
            typedef layout1D           layout_type;
            
            explicit array1D( const layout_type & L ) throw();            
            virtual ~array1D() throw() {}
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array1D);
        };
        
    }
}

#endif
