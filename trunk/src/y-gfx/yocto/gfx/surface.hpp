#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/pixel-format.hpp"

namespace yocto
{
    namespace gfx
    {
        class surface : public pixel_format, public bitmap
        {
        public:
            typedef arc_ptr<surface> pointer;
            
            class row
            {
            public:
                row(void *data, unit_t W, peek_proc proc) throw();
                const unit_t w;
                
                void *       operator[](unit_t x) throw();
                const void * operator[](unit_t x) const throw();

                
            private:
                void     *addr;    // row entry address
                peek_proc peek;    // translation
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                ~row() throw();
            };
            
            static surface *create( const pixel_format fmt, unit_t W, unit_t H);
            virtual ~surface() throw();
            
            row &       operator[]( unit_t y ) throw();
            const row & operator[]( unit_t y ) const throw();
            
            rgb_t to_rgba(const void *addr) const throw();
            
        private:
            explicit surface( const pixel_format &fmt, unit_t W, unit_t H);
            row   *rows;
            size_t nrow;
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
            
            void create_rows();
            void delete_rows() throw();
            
            
        };
    }
    
}

#endif

