#ifndef YOCTO_SLOTS_INCLUDED
#define YOCTO_SLOTS_INCLUDED 1

#include "yocto/container/cslot.hpp"
#include "yocto/type/args.hpp"
#include "yocto/code/bswap.hpp"

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
        cmem(n*sizeof(T))
        {

        }

        inline type & operator[](const size_t indx) throw()
        {
            assert(indx<size);
            return *(static_cast<type *>(cmem.data)+indx);
        }

        inline const_type operator[](const size_t indx) const throw()
        {
            assert(indx<size);
            return  *(static_cast<const_type *>(cmem.data)+indx);
        }

        //! append by copy
        inline void push_back( param_type args )
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(cmem.data)+n) mutable_type(args);
            ++n;
        }

        //! append by empty constructor
        inline void push_back()
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(cmem.data)+n) mutable_type();
            ++n;
        }

        //! append by one arg constructor
        template <typename U>
        inline void append( typename type_traits<U>::parameter_type arg1 )
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(cmem.data)+n) mutable_type(arg1);
            ++n;
        }

        //! append by one arg constructor
        template <typename U,typename V>
        inline void append(typename type_traits<U>::parameter_type arg1,
                           typename type_traits<V>::parameter_type arg2
                           )
        {
            assert(size<capacity);
            size_t &n = (size_t&)size;
            new (static_cast<mutable_type *>(cmem.data)+n) mutable_type(arg1,arg2);
            ++n;
        }

        inline virtual ~slots_of() throw()
        {
            __free();
        }

        inline bool is_empty() const throw()
        {
            return (size<=0);
        }

        //! resize if EMPTY
        inline void resize_empty_to(size_t n)
        {
            assert(size<=0);
            cslot tmp(n*sizeof(T));
            cmem.swap_with(tmp);
            (size_t&)capacity = n;
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(slots_of);
        cslot         cmem;

    protected:
        inline void __swap_with( slots_of &other ) throw()
        {
            cswap_const(size,     other.size);
            cswap_const(capacity, other.capacity);
            cmem.swap_with(other.cmem);
        }

        inline void __free() throw()
        {
            size_t &n = (size_t&)size;
            while(n>0)
            {
                --n;
                destruct( static_cast<mutable_type *>(cmem.data)+n );
            }
        }

    };


    template <typename T>
    class dynamic_slots : public slots_of<T>
    {
    public:
        inline explicit dynamic_slots(size_t n=0) : slots_of<T>(n) {}
        inline virtual ~dynamic_slots() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(dynamic_slots);

    };


}



#endif
