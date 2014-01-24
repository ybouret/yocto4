#ifndef YOCTO_SEQUENCE_SOME_ARRAYS_INCLUDED
#define YOCTO_SEQUENCE_SOME_ARRAYS_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"
#include "yocto/sequence/multi-arrays.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto
{
    
    template <
    size_t   N,
    typename T,
    typename MEMORY_KIND
    >
    class some_arrays : public multi_arrays<T,MEMORY_KIND>
    {
    public:
        typedef multi_arrays<T,MEMORY_KIND>     multi_type;
        typedef typename multi_type::array_type array_type;
        
        //! build N empty arrays
        explicit some_arrays() throw() : multi_type()
        {
            YOCTO_STATIC_CHECK(N>0,invalid_count);
            this->link();
        }
        
        virtual ~some_arrays() throw() { this->release(); }
        
        //! for info
        virtual size_t num_arrays() const throw() { return N; }
        
        
        
    private:
        uint64_t      wksp[ YOCTO_U64_FOR_SIZE( (N*sizeof(array_type)) ) ];
        virtual void *arrays_addr() const throw() { return (void *)wksp; }
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(some_arrays);
    };
    
}

#endif

