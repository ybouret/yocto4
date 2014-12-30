#include "yocto/gfx/rawpix.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace gfx
    {
        
        void save_ppm(ios::ostream &fp, const bitmap &bmp, addr2rgb proc,  const char *comment )
        {
            assert(proc);
            fp << "P6\n";
            if(comment)
            {
                fp << "#" << comment << "\n";
            }
            const unsigned w(bmp.w);
            const unsigned h(bmp.h);
            fp("%u %u\n", w,h );
            fp << "255\n";
            const ptrdiff_t d = bmp.d;
            for(unsigned y=0;y<h;++y)
            {
                const uint8_t *addr = (const uint8_t *)(bmp.get_line(y));
                for(unsigned x=0;x<w;++x,addr+=d)
                {
                    const rgb_t C = proc(addr);
                    fp.save(&C,3);
                }
            }
        }
        
        void save_ppm(const string &fn, const bitmap &bmp, addr2rgb proc,  const char *comment)
        {
            ios::ocstream fp(fn,false);
            save_ppm(fp,bmp,proc,comment);
        }
        
        
        void save_ppm(const char *fn, const bitmap &bmp, addr2rgb proc,  const char *comment)
        {
            ios::ocstream fp(fn,false);
            save_ppm(fp,bmp,proc,comment);
        }
        
    }
    
}
