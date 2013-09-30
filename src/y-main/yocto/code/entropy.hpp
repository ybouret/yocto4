#ifndef YOCTO_CODE_ENTROPY_INCLUDED
#define YOCTO_CODE_ENTROPY_INCLUDED 1

#include "yocto/memory/buffer.hpp"

namespace yocto
{
    
    class entropy
    {
    public:
        explicit entropy() throw();
        virtual ~entropy() throw();
        
        void reset() throw();
        double operator()(void) const throw();
        
        void add( uint8_t b ) throw();
        void add( const void *buffer, size_t buflen ) throw();
        void add( const memory::ro_buffer &buf ) throw();
        
    private:
        uint32_t count[256];
        YOCTO_DISABLE_COPY_AND_ASSIGN(entropy);
    };
    
}

#endif
