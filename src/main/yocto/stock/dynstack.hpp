#ifndef YOCTO_DYNSTACK_INCLUDED
#define YOCTO_DYNSTACK_INCLUDED 1

#include "yocto/ptr/shared.hpp"
#include "yocto/memory/global.hpp"
namespace yocto
{
    template <typename T>
    class dynstack
    {
    public:
        inline virtual ~dynstack() throw()
        {
            __release();
        }

        inline dynstack() throw() :
        size_(0),
        maxi_(0),
        addr_(0)
        {}

        inline dynstack(size_t n, const as_capacity_t &) :
        size_(0),
        maxi_(0),
        addr_( memory::kind<memory::global>::acquire_as<slot_type>(maxi_) )
        {
        }

        inline dynstack(const dynstack &other) :
        size_(0),
        maxi_(other.size_),
        addr_( memory::kind<memory::global>::acquire_as<slot_type>(maxi_) )
        {
            __copy(other);
        }

        inline dynstack(const dynstack &other, const size_t n) :
        size_(0),
        maxi_(other.size_+n),
        addr_(memory::kind<memory::global>::acquire_as<slot_type>(maxi_) )
        {
            __copy(other);
        }

        



    private:
        typedef shared_ptr<T> slot_type;
        size_t     size_;
        size_t     maxi_;
        slot_type *addr_;

        YOCTO_DISABLE_ASSIGN(dynstack);

        // free all slots
        inline void __free() throw()
        {
            while(size_>0)
            {
                destruct<slot_type>(&addr_[--size_]);
            }
        }

        // free slots and release memory
        inline void __release() throw()
        {
            __free();
            memory::kind<memory::global>::release_as<slot_type>(addr_, maxi_);
        }

        // copy slots => reference
        inline void __copy(const dynstack &other) throw()
        {
            assert(this!=&other);
            assert(0==size_);
            assert(maxi_>=other.size_);
            while(size_<other.size_)
            {
                new ( &addr_[size_] ) slot_type(other.addr_[size_]);
            }
        }

    };
}

#endif

