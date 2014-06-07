#ifndef YOCTO_GFX_PIXMAP_INCLUDED
#define YOCTO_GFX_PIXMAP_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/type/args.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/functor.hpp"

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
                kill_rows();
            }
            
            //! in memory pixmap
            pixmap(unit_t w, unit_t h) :
            bitmap(sizeof(T),w,h),
            nrow(0),
            rows(0)
            {
                build_rows();
            }
            
            //! hard copy
            pixmap( const pixmap &px, const rectangle *rect = 0 ) :
            bitmap( px, rect ),
            nrow(0),
            rows(0)
            {
                build_rows();
            }
            
            //! from user
            pixmap(unit_t w, unit_t h, void *data, unit_t data_stride ) :
            bitmap( sizeof(T),w,h,data,data_stride),
            nrow(0),
            rows(0)
            {
                build_rows();
            }
            
            //! from shared bitmap
            pixmap( const bitmap::pointer &bmp, const rectangle *rect=0) :
            bitmap(bmp,rect),
            nrow(0),
            rows(0)
            {
                check_depths("pixmap", sizeof(T), "bitmap", bmp->d);
                build_rows();
            }
            
            
            //! conversion
            template <typename U>
            pixmap( const pixmap<U> &px, functor<T,TL1(U)> &filter ) :
            bitmap(sizeof(T),px.w,px.h),
            nrow(0),
            rows(0)
            {
                build_rows();
                try
                {
                    for(unit_t j=0;j<h;++j)
                    {
                        row                           &tgt = rows[j];
                        const typename pixmap<U>::row &src = px[j];
                        for(unit_t i=0;i<w;++i)
                        {
                            tgt[i] = filter(src[i]);
                        }
                    }
                }
                catch(...)
                {
                    kill_rows();
                    throw;
                }
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
            
            void kill_rows() throw()
            {
                memory::kind<memory::global>::release_as<row>(rows,nrow);
            }
            
        };
        
    }
    
}

#endif
