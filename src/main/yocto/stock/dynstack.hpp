#ifndef YOCTO_DYNSTACK_INCLUDED
#define YOCTO_DYNSTACK_INCLUDED 1

#include "yocto/ptr/shared.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/core/list.hpp"
#include "yocto/object.hpp"

namespace yocto
{

    class dynstack
    {
    public:
        struct frame_t
        {
            frame_t  *next;
            frame_t  *prev;
            ptrdiff_t bp;
            ptrdiff_t tp;
        };

        virtual ~dynstack() throw()
        {
            while(hist.size) { object::release1(hist.pop_back()); }
            while(pool.size) { object::release1(pool.query()); }
        }


    protected:
        explicit dynstack() throw() : hist(), pool() {}
        core::list_of<frame_t> hist;
        core::pool_of<frame_t> pool;

        void clean_history() throw()
        {
            while(hist.size) pool.store( hist.pop_back() );
        }

        dynstack(const dynstack &other) :
        hist(),
        pool()
        {
            for(const frame_t *f = other.hist.head;f;f=f->next)
            {
                frame_t *fc = object::acquire1<frame_t>();
                fc->bp = f->bp;
                fc->tp = f->tp;
                hist.push_back(fc);
            }
        }




    private:
        YOCTO_DISABLE_ASSIGN(dynstack);
    };

    template <typename T>
    class dynstack_of : public dynstack
    {
    public:

        inline virtual ~dynstack_of() throw()
        {
            __release();
        }

        inline dynstack_of() throw() :
        dynstack(),
        items(0),
        maxi_(0),
        addr_(0),
        base_(0),
        last_(base_+1)
        {}

        inline dynstack_of(size_t n, const as_capacity_t &) :
        dynstack(),
        items(0),
        maxi_(0),
        addr_( memory::kind<memory::global>::acquire_as<slot_type>(maxi_) ),
        base_(addr_),
        last_(base_+1)
        {
        }

        inline dynstack_of(const dynstack_of &other) :
        dynstack(other),
        items(0),
        maxi_(other.size_),
        addr_( memory::kind<memory::global>::acquire_as<slot_type>(maxi_) ),
        base_(addr_),
        last_(base_+1)
        {
            __copy(other);
        }

        inline dynstack_of(const dynstack_of &other, const size_t n) :
        items(0),
        maxi_(other.items+n),
        addr_(memory::kind<memory::global>::acquire_as<slot_type>(maxi_) ),
        base_(addr_),
        last_(base_+1)
        {
            __copy(other);
        }

        //! #objects in current frame
        inline size_t size() const throw()
        {
            assert(last_>base_);
            assert(base_>=addr_);
            return size_t(static_cast<ptrdiff_t>(last_-base_)-1);
        }





    private:
        typedef shared_ptr<T> slot_type;
        size_t     items;
        size_t     maxi_;
        slot_type *addr_;
        slot_type *base_;
        slot_type *last_;

        YOCTO_DISABLE_ASSIGN(dynstack_of);

        // free all slots
        inline void __free() throw()
        {
            while(items>0)
            {
                destruct<slot_type>(&addr_[--items]);
            }
            base_ = last_ = addr_;
            clean_history();
        }

        // free slots and release memory
        inline void __release() throw()
        {
            __free();
            memory::kind<memory::global>::release_as<slot_type>(addr_, maxi_);
            base_ = last_ = 0;
        }

        // copy slots => reference
        inline void __copy(const dynstack_of &other) throw()
        {
            assert(this!=&other);
            assert(0==items);
            assert(maxi_>=other.items);
            while(items<other.items)
            {
                new ( &addr_[items] ) slot_type(other.addr_[items]);
                ++items;
            }
            base_ = addr_+static_cast<ptrdiff_t>(other.base_-other.addr_);
            last_ = addr_+static_cast<ptrdiff_t>(other.last_-other.addr_);
        }

    };
}

#endif

