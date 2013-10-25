#ifndef YOCTO_PIXMAP_INCLUDED
#define YOCTO_PIXMAP_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class pixmap_base
        {
        public:
            virtual ~pixmap_base() throw();
            
            static void check_same( size_t Depth, size_t SizeOf );
            
        protected:
            explicit pixmap_base();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixmap_base);
        };
        
        template <typename T>
        class pixmap
        {
        public:
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixmap);
        };
    }
}


#endif

