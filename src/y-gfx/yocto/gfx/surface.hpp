#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/color-format.hpp"
#include "yocto/gfx/pixel-buffer.hpp"

namespace yocto
{
    namespace gfx
    {
        class surface
        {
        public:
            class line
            {
            public:
                line( void *entry, const surface &s ) throw();
                const size_t w;
                
                void *       operator[](size_t x) throw();
                const void * operator[](size_t x) const throw();
                
                
            private:
                void                   *addr;
                const pixbuf::peek_proc peek;
                ~line() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(line);
            };
            
            const color_format format;
            pixel_buffer       pixels;
            const size_t       w;
            const size_t       h;
            
            virtual ~surface() throw();
            explicit surface( const color_format fmt, size_t width, size_t height);
            
            line       & operator[](size_t y) throw(); //!< 0<=y<h
            const line & operator[](size_t y) const throw(); //!< 0<=y<h
            
            
            pixel_t __get( const void * ) const throw();
            pixel_t get_pixel( size_t x, size_t y ) const throw();
            
            void  __put( void *, pixel_t ) throw();
            void    put_pixel( size_t x, size_t y, pixel_t C) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
            size_t nline;
            line  *lines;
            void link( unit_t xoff, unit_t yoff ) throw();
            
        };
    }
}

#endif

