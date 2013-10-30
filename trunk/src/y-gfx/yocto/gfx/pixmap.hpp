#ifndef YOCTO_PIXMAP_INCLUDED
#define YOCTO_PIXMAP_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"

namespace yocto
{
    namespace gfx
    {
        
        void pixmap_check_same( size_t depth, size_t sz );
        
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef arc_ptr<pixmap> pointer;
            
            class row
            {
            public:
                inline row(void *P, unit_t W) throw():
                w(W), addr((mutable_type *)P)
                {
                    assert(w>0);
                    assert(0!=addr);
                }
                
                const unit_t w;
                
                inline type & operator[](unit_t x) throw()
                {
                    assert(x>=0);
                    assert(x<w);
                    return addr[x];
                }
                
                inline const_type & operator[](unit_t x) const throw()
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
            
            virtual ~pixmap() throw() { delete_rows(); }
            
            
            static inline pixmap *create(unit_t W, unit_t H)
            {
                return new pixmap(W,H);
            }
            
            static inline pixmap *attach( bitmap &bmp, const region *rect )
            {
                if(rect)
                    return new pixmap(bmp,*rect);
                else
                {
                    const region full(0,0,bmp.w,bmp.h);
                    return new pixmap(bmp,full);
                }
            }
            
            static inline pixmap *carbon( const bitmap &bmp, const region *rect )
            {
                pixmap_check_same(bmp.depth, sizeof(T));
                const region  full(0,0,bmp.w,bmp.h);
                const region &area = rect ? *rect : full;
                pixmap       *pxm  = new pixmap(area.w,area.h);
                pxm->copy_data_from(bmp,area);
                return pxm;
            }
            
        protected:
            inline pixmap(unit_t W, unit_t H ) :
            bitmap(sizeof(T),W,H),
            nrow(0),
            rows(0)
            {
                create_rows();
            }
            
            inline pixmap( bitmap &bmp, const region &rect ) :
            bitmap( validate(bmp),rect),
            nrow(0),
            rows()
            {
                create_rows();
            }

            
        private:
            size_t nrow;
            row   *rows;
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixmap);
            
            inline void create_rows()
            {
                nrow = h;
                rows = memory::kind<memory::global>::acquire_as<row>(nrow);
                uint8_t *p = static_cast<uint8_t*>(entry);
                for(unit_t i=0;i<h;++i,p+=this->stride)
                    new (&rows[i]) row(p,w);
            }
            
            inline void delete_rows() throw()
            {
                memory::kind<memory::global>::release_as<row>(rows,nrow);
            }
            
            inline bitmap  & validate( bitmap &bmp )
            {
                pixmap_check_same(bmp.depth, sizeof(T));
                return bmp;
            }
        };
    }
}


#endif

