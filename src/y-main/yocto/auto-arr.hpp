#ifndef YOCTO_AUTO_ARR_INCLUDED
#define YOCTO_AUTO_ARR_INCLUDED 1

#include "yocto/memory/allocator.hpp"
#include "yocto/type/args.hpp"
#include "yocto/code/bswap.hpp"

#include <cstring>

namespace yocto
{
    
    //! for C-style inline arrays
    template <
    typename T,
    typename ALLOCATOR = memory::global
    >
    class auto_arr
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        
        const size_t size;
        
        explicit auto_arr( size_t n ) :
        size(n),
        addr( memory::kind<ALLOCATOR>::template acquire_as<mutable_type>( (size_t&)size ))
        {
        }
        
        virtual ~auto_arr() throw()
        {
            memory::kind<ALLOCATOR>::template release_as<mutable_type>( addr, (size_t&)size );
        }
        
        inline void swap_with( auto_arr &other ) throw()
        {
            cswap(addr,other.addr);
            cswap_const(size,other.size);
        }
        
        void reserve( size_t n )
        {
            if(n>0)
            {
                auto_arr tmp( size+n );
                memcpy( tmp.addr, addr, size * sizeof(T) );
                swap_with(tmp);
            }
        }
        
        inline type *      base() throw()       { return addr; }
        inline const_type *base() const throw() { return addr; }
        
        inline type       & operator[](size_t i) throw()       { assert(i<size); return addr[i]; }
        inline const_type & operator[](size_t i) const throw() { assert(i<size); return addr[i]; }
        
    private:
        mutable_type *addr;
        YOCTO_DISABLE_COPY_AND_ASSIGN(auto_arr);
    };
    
}


#endif
