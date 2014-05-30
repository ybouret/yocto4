#ifndef YOCTO_GFX_PIXMAP_INCLUDED
#define YOCTO_GFX_PIXMAP_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/type/args.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
    
    namespace gfx
    {
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            
            //! pre-computed row
            class row
            {
            public:
                inline  row(void *p,unit_t width) throw() :
                w(width), addr( (mutable_type*)p ) {}
                
                const unit_t w;
                inline type &operator[](unit_t x) throw()
                {
                    assert(x>=0);
                    assert(x<w);
                    return addr[x];
                }
                
                inline const_type &operator[](unit_t x) const throw()
                {
                    assert(x>=0);
                    assert(x<w);
                    return addr[x];
                }
                
            private:
                mutable_type *addr;
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                ~row() throw();
            };
            
            
            inline row & operator[](unit_t y) throw()
            {
                assert(y>=0);
                assert(y<h);
                return rows[y];
            }
            
            inline const row & operator[](unit_t y) const throw()
            {
                assert(y>=0);
                assert(y<h);
                return rows[y];
            }
            
            inline virtual ~pixmap() throw()
            {
                memory::kind<memory::global>::release_as<row>(rows,nrow);
            }
            
            //! in memory pixmap
            pixmap(unit_t w, unit_t h) :
            bitmap(sizeof(T),w,h)
            {
                build_rows();
            }
            
            //! hard copy
            pixmap( const pixmap &px, const rectangle *rect = 0 ) :
            bitmap( px, rect )
            {
                build_rows();
            }
            
            //! from user
            pixmap(unit_t w, unit_t h, void *data, unit_t data_stride ) :
            bitmap( sizeof(T),w,h,data,data_stride)
            {
                build_rows();
            }
            
            
            
        private:
            size_t nrow;
            row   *rows;
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixmap);
            inline void build_rows()
            {
                nrow = h;
                rows = memory::kind<memory::global>::acquire_as<row>(nrow);
                uint8_t *p = (uint8_t*)entry;
                for(unit_t y=0;y<h;++y)
                {
                    new (rows+y) row(p,w);
                    p += stride;
                }
            }
            
        };
        
    }
    
}

#endif
