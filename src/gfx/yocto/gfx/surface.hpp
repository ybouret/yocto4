#ifndef YOCTO_GFX_SURFACE_INCLUDED
#define YOCTO_GFX_SURFACE_INCLUDED 1

#include "yocto/gfx/pixel-format.hpp"
#include "yocto/gfx/bitmap.hpp"


namespace yocto
{
    namespace gfx
    {
        
        class surface : public bitmap
        {
        public:
            class row
            {
            public:
                row(void *addr, const surface *surf) throw();
                
                void *       operator[](unit_t i) throw();
                const void * operator[](unit_t i) const throw();
                
            private:
                void           *p;
                const surface  *s;
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                inline ~row() throw();
            };
            
            const pixel_format format;
            
            virtual ~surface() throw();
            explicit surface( const pixel_format fmt, unit_t width, unit_t height);
            
            row & operator[](unit_t j) throw();
            const row & operator[](unit_t j) const throw();
            
            void put(void *addr, const pixel_t C);
            void put(void *addr, const rgb_t  &C);
            void put(void *addr, const rgba_t &C);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(surface);
            size_t nrow;
            row   *rows;
            void make_rows();
            void kill_rows() throw();
        };
        
    }
}

#endif
