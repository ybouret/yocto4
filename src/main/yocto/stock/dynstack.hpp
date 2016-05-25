#ifndef YOCTO_DYNSTACK_INCLUDED
#define YOCTO_DYNSTACK_INCLUDED 1

#include "yocto/ptr/shared.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/core/list.hpp"
#include "yocto/object.hpp"
#include "yocto/container/container.hpp"
#include <iostream>

namespace yocto
{

    class dynstack
    {
    public:
        struct frame_t
        {
            frame_t  *next;
            frame_t  *prev;
            ptrdiff_t head;
            ptrdiff_t tail;
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
                fc->head = f->head;
                fc->tail = f->tail;
                hist.push_back(fc);
            }
        }

        void save(const ptrdiff_t head,
                  const ptrdiff_t tail)
        {
            frame_t *f = pool.size ? pool.query() : object::acquire1<frame_t>();
            f->head = head;
            f->tail = tail;
            hist.push_back(f);
        }


    private:
        YOCTO_DISABLE_ASSIGN(dynstack);
    };

#define YOCTO_DYNSTACK_CTOR() \
head_(addr_   ),  \
tail_(head_-1 ),  \
sub_( head_-1 ),  \
top_( tail_+1 )

#define YOCTO_DYNSTACK_CHECK() \
assert(head_>=addr_);          \
assert(head_-1==sub_);         \
assert(tail_+1==top_)

    template <typename T>
    class dynstack_of : public dynstack
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        inline virtual ~dynstack_of() throw()
        {
            __release();
        }

        inline dynstack_of() throw() :
        dynstack(),
        items(0),
        maxi_(0),
        addr_(0),
        YOCTO_DYNSTACK_CTOR()
        {
            YOCTO_DYNSTACK_CHECK();
        }

        inline dynstack_of(size_t n, const as_capacity_t &) :
        dynstack(),
        items(0),
        maxi_(n),
        addr_( memory::kind<memory::global>::acquire_as<slot_type>(maxi_) ),
        YOCTO_DYNSTACK_CTOR()
        {
            YOCTO_DYNSTACK_CHECK();
        }

        inline dynstack_of(const dynstack_of &other) :
        dynstack(other),
        items(0),
        maxi_(other.size_),
        addr_( memory::kind<memory::global>::acquire_as<slot_type>(maxi_) ),
        YOCTO_DYNSTACK_CTOR()
        {
            YOCTO_DYNSTACK_CHECK();
            __copy(other);
            YOCTO_DYNSTACK_CHECK();
        }

        inline dynstack_of(const dynstack_of &other, const size_t n) :
        items(0),
        maxi_(other.items+n),
        addr_(memory::kind<memory::global>::acquire_as<slot_type>(maxi_) ),
        YOCTO_DYNSTACK_CTOR()
        {
            YOCTO_DYNSTACK_CHECK();
            __copy(other);
            YOCTO_DYNSTACK_CHECK();
        }

        inline void swap_with( dynstack_of &other ) throw()
        {
            hist.swap_with(other.hist);
            pool.swap_with(other.pool);
            cswap(items,other.items);
            cswap(maxi_,other.maxi_);
            cswap(addr_,other.addr_);
            cswap(head_,other.head_);
            cswap(tail_,other.tail_);
            cswap(sub_, other.sub_);
            cswap(top_, other.top_);
            YOCTO_DYNSTACK_CHECK();
        }

        //! #objects in current frame
        inline size_t size() const throw()
        {
            YOCTO_DYNSTACK_CHECK();
            return size_t(static_cast<ptrdiff_t>(top_-head_));
        }

        inline void save_frame()
        {
            YOCTO_DYNSTACK_CHECK();
            this->save(static_cast<ptrdiff_t>(head_-addr_),
                       static_cast<ptrdiff_t>(tail_-addr_));
        }

        inline void reserve(size_t n)
        {
            std::cerr << "reserve(" << n << ")" << std::endl;
            if(n>0)
            {
                dynstack_of tmp(*this,n);
                swap_with(tmp);
                YOCTO_DYNSTACK_CHECK();
            }
        }

        inline void push( param_type arg )
        {
            if(items>=maxi_)
            {
                reserve( container::next_increase(maxi_) );
            }
            YOCTO_DYNSTACK_CHECK();
            assert(top_<addr_+maxi_);
            new (top_) slot_type( new type(arg) );
            ++top_;
            ++tail_;
            ++items;
        }

        inline void pop() throw()
        {
            assert(tail_>=head_);
            assert(size()>0);
            destruct<slot_type>(tail_);
            --tail_;
            --top_;
            --items;
        }


    private:
        typedef shared_ptr<T> slot_type;
        size_t     items;
        size_t     maxi_;
        slot_type *addr_;

        slot_type *head_;
        slot_type *tail_;

        slot_type *sub_;
        slot_type *top_;

        YOCTO_DISABLE_ASSIGN(dynstack_of);

        inline void reset() throw()
        {
            head_ = addr_;
            tail_ = head_-1;
            sub_  = head_-1;
            top_  = tail_+1;
            YOCTO_DYNSTACK_CHECK();
        }


        // free all slots
        inline void __free() throw()
        {
            while(items>0)
            {
                destruct<slot_type>(&addr_[--items]);
            }
            reset();
            clean_history();
        }

        // free slots and release memory
        inline void __release() throw()
        {
            __free();
            memory::kind<memory::global>::release_as<slot_type>(addr_, maxi_);
            reset();
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
            head_ = addr_ + static_cast<ptrdiff_t>(other.head_-other.addr_);
            tail_ = addr_ + static_cast<ptrdiff_t>(other.tail_-other.addr_);
            sub_  = head_-1;
            top_  = tail_+1;
            YOCTO_DYNSTACK_CHECK();
        }

    };
}

#endif

