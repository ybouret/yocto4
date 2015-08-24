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

        inline type & operator[](const size_t indx) throw()
        {
            assert(indx>0);
            assert(indx<=size);
            return addr[indx];
        }

        inline const_type operator[](const size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=size);
            return addr[indx];
        }

        //! append by copy
        inline void push_back( param_type args )
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(mem.data)+n) mutable_type(args);
            ++n;
        }

        //! append by empty constructor
        inline void push_back()
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(mem.data)+n) mutable_type();
            ++n;
        }

        //! append by one arg constructor
        template <typename U>
        inline void append( typename type_traits<U>::param_type arg1 )
        {
            
        }

        inline virtual ~slots_of() throw()
        {
            free();
        }

    private:
        cslot mem;
        YOCTO_DISABLE_COPY_AND_ASSIGN(slots_of);

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
    };
}



#endif
