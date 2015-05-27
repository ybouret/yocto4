#ifndef YOCTO_NET_IO_BLOCK_INCLUDED
#define YOCTO_NET_IO_BLOCK_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/object.hpp"

namespace yocto
{
    namespace network
    {
        class io_block
        {
        public:
            io_block *next;
            io_block *prev;
            
            io_block( const size_t block_size );
            ~io_block() throw();

            const size_t   buflen;
            uint8_t       *buffer;


            YOCTO_MAKE_OBJECT

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(io_block);
        };
    }
}

#endif
