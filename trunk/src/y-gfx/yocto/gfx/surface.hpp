#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/format.hpp"

namespace yocto
{
    namespace gfx
    {
        class surface : public format, public bitmap
        {
        public:
            typedef arc_ptr<surface> pointer;
            
            class row
            {
            public:
                row(void *addr, unit_t W) throw();
                const unit_t w;
                
            private:
                void *p; // row entry address
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                ~row() throw();
            };
            
            static surface *create( const format fmt, unit_t W, unit_t H);
            virtual ~surface() throw();
            
        private:
            explicit surface( const format &fmt, unit_t W, unit_t H);
            row   *rows;
            size_t nrow;
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
            
            void create_rows();
            void delete_rows() throw();
            
        };
    }
    
}

#endif

