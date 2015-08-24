#ifndef YOCTO_SLOTS_INCLUDED
#define YOCTO_SLOTS_INCLUDED 1

#include "yocto/container/cslot.hpp"
#include "yocto/type/args.hpp"

namespace yocto
{
    template <typename T>
    class slots_of
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        const size_t size;
        const size_t capacity;

        inline explicit slots_of(size_t n) :
        size(0),
        capacity(n),
        mem(n*sizeof(T)),
        addr( static_cast<mutable_type *>(mem.data)-1 )
        {

        }


        inline virtual ~slots_of() throw()
        {
        }

    protected:
        mutable_type *addr;

        inline void free() throw()
        {
            size_t &n = (size_t&)size;
            for(;n>0;--n)
            {
                destruct(addr+n);
            }
        }

    private:
        cslot mem;
        YOCTO_DISABLE_COPY_AND_ASSIGN(slots_of);
    };
}



#endif
