#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/color-format.hpp"
#include "yocto/gfx/pixel-buffer.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class surface : public object
        {
        public:
            
            class row
            {
            public:
                row(size_t width, void *start, const size_t BPP) throw();
                const size_t w;
                
                void * operator[](size_t i) throw();
                const void * operator[](size_t i) const throw();
                
            private:
                void        *p;
                void  *    (*move)(void *,size_t);
                
                ~row() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
            };
            
            //! make a new memory surface
            explicit surface( const color_format &fmt, size_t width, size_t height);
            virtual ~surface() throw();
            
            
            row &       operator[](size_t j) throw();
            const row & operator[](size_t j) const throw();

            const color_format format;
            const pixel_buffer pixels;
            const size_t       w;
            const size_t       h;
            
            void save_ppm( const string &filename ) const;
            
        private:
            size_t nrow;
            row   *rows;
            void   link( unit_t xoff, unit_t yoff ) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
        };
    }
}

#endif


