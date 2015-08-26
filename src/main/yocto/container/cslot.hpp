#ifndef YOCTO_CSLOT_INCLUDED
#define YOCTO_CSLOT_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
    //! C-data slot: managing memory
    class cslot
    {
    public:
        cslot() throw();
        ~cslot() throw();
        cslot(size_t n);
        void swap_with( cslot &other ) throw();
        
        void        *data; //!< address
        const size_t size; //!< size in bytes
        
        void prepare(size_t n);
        void release() throw();
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(cslot);
    };
}


#endif


