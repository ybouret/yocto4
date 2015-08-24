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

        inline explicit slots_of(size_t n=0) :
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
        inline void append( typename type_traits<U>::parameter_type arg1 )
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(mem.data)+n) mutable_type(arg1);
            ++n;
        }

        //! append by one arg constructor
        template <typename U,typename V>
        inline void append( typename type_traits<U>::parameter_type arg1,
                            typename type_traits<V>::parameter_type arg2
                           )
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(mem.data)+n) mutable_type(arg1,arg2);
            ++n;
        }

        inline virtual ~slots_of() throw()
        {
            __free();
        }

        //! resize if EMPTY
        inline void resize(size_t n)
        {
            assert(size<=0);
            cslot tmp(n*sizeof(T));
            mem.swap_with(tmp);
            addr = static_cast<mutable_type *>(mem.data)-1;
            (size_t&)capacity = n;
        }

    private:
        cslot mem;
        YOCTO_DISABLE_COPY_AND_ASSIGN(slots_of);

    protected:
        mutable_type *addr;

        inline void __free() throw()
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
