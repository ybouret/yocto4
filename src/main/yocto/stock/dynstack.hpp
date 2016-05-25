#ifndef YOCTO_DYNSTACK_INCLUDED
#define YOCTO_DYNSTACK_INCLUDED 1

#include "yocto/ptr/shared.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/core/list.hpp"
#include "yocto/object.hpp"
#include "yocto/container/container.hpp"

namespace yocto
{

    namespace hidden
    {
        extern const char dynstack_name[];
    }

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

        virtual ~dynstack() throw();

    protected:
        explicit dynstack() throw();
        dynstack(const dynstack &other);

        core::list_of<frame_t> hist;
        core::pool_of<frame_t> pool;

        void clean_history() throw();


        void save(const ptrdiff_t head,
                  const ptrdiff_t tail);

    private:
        YOCTO_DISABLE_ASSIGN(dynstack);
    };

#define YOCTO_DYNSTACK_CTOR() \
head_(addr_   ),              \
tail_(head_-1 ),              \
sub_( head_-1 ),              \
top_( tail_+1 )

#define YOCTO_DYNSTACK_MAKE()                                       \
addr_(memory::kind<memory::global>::acquire_as<slot_type>(maxi_)),  \
YOCTO_DYNSTACK_CTOR()

#define YOCTO_DYNSTACK_CHECK() \
assert(head_>=addr_);          \
assert(head_-1==sub_);         \
assert(tail_+1==top_)

    template <typename T>
    class dynstack_of : public container, public dynstack
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        inline virtual ~dynstack_of() throw()
        {
            __release();
        }

        inline dynstack_of() throw() :
        dynstack(),
        itms_(0),
        maxi_(0),
        addr_(0),
        YOCTO_DYNSTACK_CTOR()
        {
            YOCTO_DYNSTACK_CHECK();
        }

        inline dynstack_of(size_t n, const as_capacity_t &) :
        dynstack(),
        itms_(0),
        maxi_(n),
        YOCTO_DYNSTACK_MAKE()
        {
            YOCTO_DYNSTACK_CHECK();
        }

        inline dynstack_of(const dynstack_of &other) :
        dynstack(other),
        itms_(0),
        maxi_(other.size_),
        YOCTO_DYNSTACK_MAKE()
        {
            YOCTO_DYNSTACK_CHECK();
            __copy(other);
            YOCTO_DYNSTACK_CHECK();
        }

        inline dynstack_of(const dynstack_of &other, const size_t n) :
        itms_(0),
        maxi_(other.itms_+n),
        YOCTO_DYNSTACK_MAKE()
        {
            YOCTO_DYNSTACK_CHECK();
            __copy(other);
            YOCTO_DYNSTACK_CHECK();
        }

        inline void swap_with( dynstack_of &other ) throw()
        {
            hist.swap_with(other.hist);
            pool.swap_with(other.pool);
            cswap(itms_,other.itms_);
            cswap(maxi_,other.maxi_);
            cswap(addr_,other.addr_);
            cswap(head_,other.head_);
            cswap(tail_,other.tail_);
            cswap(sub_, other.sub_);
            cswap(top_, other.top_);
            YOCTO_DYNSTACK_CHECK();
        }

        //! #objects in current frame
        inline size_t height() const throw()
        {
            YOCTO_DYNSTACK_CHECK();
            return size_t(static_cast<ptrdiff_t>(top_-head_));
        }

        inline virtual size_t capacity() const throw()
        {
            return maxi_;
        }

        inline virtual size_t size() const throw()
        {
            return itms_;
        }

        inline void start_frame()
        {
            YOCTO_DYNSTACK_CHECK();
            this->save(static_cast<ptrdiff_t>(head_-addr_),
                       static_cast<ptrdiff_t>(tail_-addr_));
            head_ = top_;
            tail_ = head_-1;
            sub_  = head_-1;
        }

        inline virtual void reserve(size_t n)
        {
            if(n>0)
            {
                dynstack_of tmp(*this,n);
                swap_with(tmp);
                YOCTO_DYNSTACK_CHECK();
            }
        }

        inline void push( param_type arg )
        {
            if(itms_>=maxi_)
            {
                reserve( container::next_increase(maxi_) );
            }
            YOCTO_DYNSTACK_CHECK();
            assert(top_<addr_+maxi_);
            new (top_) slot_type( new type(arg) );
            ++top_;
            ++tail_;
            ++itms_;
        }

        inline void pop() throw()
        {
            assert(tail_>=head_);
            assert(height()>0);
            destruct<slot_type>(tail_);
            --tail_;
            --top_;
            --itms_;
        }

        //! free current frame
        inline void clear() throw()
        {
            while(tail_>=head_)
            {
                pop();
            }
        }

        inline virtual void free() throw()
        {
            __free_all();
        }

        inline virtual void release() throw()
        {
            __release();
        }

        inline virtual const char *name() const throw() { return hidden::dynstack_name; }

    private:
        typedef shared_ptr<T> slot_type;
        size_t     itms_;
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
        inline void __free_all() throw()
        {
            while(itms_>0)
            {
                destruct<slot_type>(&addr_[--itms_]);
            }
            reset();
            clean_history();
        }

        // free slots and release memory
        inline void __release() throw()
        {
            __free_all();
            memory::kind<memory::global>::release_as<slot_type>(addr_, maxi_);
            reset();
        }

        // copy slots => reference
        inline void __copy(const dynstack_of &other) throw()
        {
            assert(this!=&other);
            assert(0==itms_);
            assert(maxi_>=other.itms_);
            while(itms_<other.itms_)
            {
                new ( &addr_[itms_] ) slot_type(other.addr_[itms_]);
                ++itms_;
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

