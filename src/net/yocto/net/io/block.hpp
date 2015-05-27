#ifndef YOCTO_NET_IO_BLOCK_INCLUDED
#define YOCTO_NET_IO_BLOCK_INCLUDED 1

#include "yocto/object.hpp"

namespace yocto
{
    namespace network
    {

        class io_socket;
        
        class io_block
        {
        public:
            io_block *next;
            io_block *prev;
            
            io_block( const size_t block_size );
            ~io_block() throw();

            const size_t   len;

            size_t shift() const throw(); //!< how many consumed bytes
            size_t bytes() const throw(); //!< stored bytes
            size_t space() const throw(); //!< available space

            void initialize() throw(); //!< reset pointers
            void defragment() throw(); //!< optimize space


            size_t load( io_socket &iosock );
            size_t emit( io_socket &iosock );

            YOCTO_MAKE_OBJECT

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(io_block);
            uint8_t       *blk;
            uint8_t       *r;
            uint8_t       *w;
            const uint8_t *last;
        };
    }
}

#endif
