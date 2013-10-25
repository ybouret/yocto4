#include "yocto/gfx/bitmap.hpp"
#include "yocto/exception.hpp"
#include "yocto/memory/global.hpp"

#include <cstring>

namespace yocto
{
    namespace gfx
    {
        bitmap:: ~bitmap() throw()
        {
            assert(entry);
            switch(type)
            {
                case in_memory:
                    assert(allocated>0);
                    assert(0==shared);
                    memory::kind<memory::global>::release(entry, (size_t&)allocated );
                    break;
                    
                case from_data:
                    // do nothing
                    assert(0==allocated);
                    assert(0==shared);
                    break;
                    
                case is_shared:
                    assert(0==allocated);
                    assert(0!=shared);
                    if(shared->liberate())
                    {
                        delete shared;
                    }
                    break;
            }
            memset( (void*)&w, 0, 1);
        }
        
        
        static inline
        size_t __check_bitmap( size_t Value, const char *Name )
        {
            if(Value<=0)
                throw exception("Negative Bitmap %s",Name);
            return Value;
        }
        
        bitmap:: bitmap( size_t Depth, size_t W, size_t H ) :
        type( in_memory ),
        depth( __check_bitmap(Depth,"Depth" ) ),
        w(     __check_bitmap(W,    "Width" ) ),
        h(     __check_bitmap(H,    "Height") ),
        pitch( w*depth ),
        stride( pitch ),
        entry(0),
        allocated( stride * h ),
        shared(0)
        {
            entry = memory::kind<memory::global>::acquire( (size_t&)allocated );
        }
        
        void bitmap:: ldz() throw()
        {
            uint8_t *p = (uint8_t*)entry;
            for(size_t i=h;i>0;--i)
            {
                memset(p,0,pitch);
                p += stride;
            }
        }
        
        bitmap:: bitmap( const bitmap::pointer &bmp, unit_t x, unit_t y, size_t w, size_t h ) :
        type( is_shared ),
        depth(bmp->depth),
        w( __check_bitmap(w, "Shared Width") ),
        h( __check_bitmap(h, "Shared Height")),
        pitch( w * depth ),
        stride( bmp->stride ),
        entry(0),
        allocated(0),
        shared(0)
        {
            if(x<=0||x+w>=bmp->w) throw exception("Invalid Shared Bitmap x-offset");
            if(y<=0||y+h>=bmp->h) throw exception("Invalid Shared Bitmap y-offset");
            
            shared = (bitmap *)( bmp.__get() );
            shared->withhold();
            
            uint8_t *p = static_cast<uint8_t*>(shared->entry);
            p += y * stride;
            p += x * depth;
            entry = p;
            
        }
        

        
        
    }
}
