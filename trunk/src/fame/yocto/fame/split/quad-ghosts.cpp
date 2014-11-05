#include "yocto/fame/split/quad-ghosts.hpp"

namespace yocto
{
    namespace fame
    {
        quad_ghosts_io:: quad_ghosts_io() throw() :
        sbuffer(0),
        rbuffer(0),
        io_size(0),
        iobytes(0)
        {
        }
        
        quad_ghosts_io:: ~quad_ghosts_io() throw()
        {
            release();
        }
        
        
        void quad_ghosts_io:: release() throw()
        {
            memory::kind<memory::global>::release(sbuffer,iobytes);
            io_update();
        }
        
        void quad_ghosts_io:: io_update() throw()
        {
            (size_t &)io_size = iobytes/2;
            uint8_t *wksp = (uint8_t *)sbuffer;
            rbuffer = &wksp[io_size];
        }
        
        void quad_ghosts_io:: ensure(size_t interleaved_bytes)
        {
            if(interleaved_bytes>io_size)
            {
                release();
                iobytes = 2*interleaved_bytes;
                sbuffer = memory::kind<memory::global>::acquire(iobytes);
                io_update();
            }
        }

        
    }
}
