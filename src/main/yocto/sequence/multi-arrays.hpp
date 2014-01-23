#ifndef YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED
#define YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    
    template <
    size_t   N,
    typename T,
    typename MEMORY_KIND
    >
    class multi_arrays
    {
    public:
        typedef lw_array<T> array_type;
        
        explicit multi_arrays() throw()
        {
        }
        
        virtual ~multi_arrays() throw() {}
        
    private:
        uint64_t wksp[ YOCTO_U64_FOR_SIZE(N*sizeof(array_type)) ];
        YOCTO_DISABLE_COPY_AND_ASSIGN(multi_arrays);
    };
    
}

#endif

