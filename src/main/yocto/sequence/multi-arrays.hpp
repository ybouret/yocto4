#ifndef YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED
#define YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"
#include "yocto/sequence/arrays-mgr.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto
{
    
    template <
    size_t   N,
    typename T,
    typename MEMORY_KIND
    >
    class multi_arrays : public arrays_mgr<T,MEMORY_KIND>
    {
    public:
        typedef arrays_mgr<T,MEMORY_KIND>     mgr_type;
        typedef typename mgr_type::array_type array_type;
        
        //! build N empty arrays
        explicit multi_arrays() throw() : mgr_type()
        {
            YOCTO_STATIC_CHECK(N>0,invalid_count);
            this->link();
        }
        
        //! build N arrays with same size=nvar
        explicit multi_arrays(size_t nvar) : mgr_type()
        {
            YOCTO_STATIC_CHECK(N>0,invalid_count);
            this->allocate(nvar);
        }
        
        virtual ~multi_arrays() throw() { this->release(); }
        
        
        //! for info
        virtual size_t num_arrays() const throw() { return N; }
        
        
        
    private:
        uint64_t      wksp[ YOCTO_U64_FOR_SIZE( (N*sizeof(array_type)) ) ];
        virtual void *arrays_addr() const throw() { return (void *)wksp; }
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(multi_arrays);
    };
    
}

#endif

