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

    namespace hidden
    {
        extern const char dynstack_name[];
    }

    class stackmgr
    {
    public:
        struct frame_t
        {
            frame_t  *next;
            frame_t  *prev;
            ptrdiff_t head;
            ptrdiff_t tail;
        };

        virtual ~stackmgr() throw();

        size_t frames() const throw();
        
    protected:
        explicit stackmgr() throw();
        stackmgr(const stackmgr &other);

        core::list_of<frame_t> hist;
        core::pool_of<frame_t> pool;

        void clean_history() throw();


        void save(const ptrdiff_t head,
                  const ptrdiff_t tail);

    private:
        YOCTO_DISABLE_ASSIGN(stackmgr);
    };



    template <typename T>
    class dynstack
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        virtual ~dynstack() throw() {}

        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual ptrdiff_t    height() const throw() = 0;
        virtual void         push( param_type )     = 0;
        virtual void         pop()  throw()         = 0;
        virtual type &       peek() throw()         = 0;
        virtual const_type & peek() const throw()   = 0;
        virtual type &       base() throw()         = 0;
        virtual const_type & base() const throw()   = 0;
        virtual type       & operator[](ptrdiff_t n) throw() = 0;
        virtual const_type & operator[](ptrdiff_t n) const throw() = 0;

        //______________________________________________________________________
        //
        // non virtual interface
        //______________________________________________________________________

        //! free current frame
        inline void clear() throw()
        {
            while( height() > 0 )
            {
                pop();
            }
        }



    protected:
        explicit dynstack() throw() {}

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(dynstack);
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
    class dynstack_of : public container, public stackmgr, public dynstack<T>
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        //! destructor
        inline virtual ~dynstack_of() throw()
        {
            __release();
        }

        //! default ctor: make empty stack
        inline dynstack_of() throw() :
        stackmgr(),
        itms_(0),
        maxi_(0),
        addr_(0),
        YOCTO_DYNSTACK_CTOR()
        {
            YOCTO_DYNSTACK_CHECK();
        }

        //! ctor to acquire some memory
        inline dynstack_of(size_t n, const as_capacity_t &) :
        stackmgr(),
        itms_(0),
        maxi_(n),
        YOCTO_DYNSTACK_MAKE()
        {
            YOCTO_DYNSTACK_CHECK();
        }

        //! SOFT copy ctor
        inline dynstack_of(const dynstack_of &other) :
        stackmgr(other),
        itms_(0),
        maxi_(other.size_),
        YOCTO_DYNSTACK_MAKE()
        {
            YOCTO_DYNSTACK_CHECK();
            __copy(other);
            YOCTO_DYNSTACK_CHECK();
        }

        //! SOFT copy ctor with extra memory
        inline dynstack_of(const dynstack_of &other, const size_t n) :
        stackmgr(other),
        itms_(0),
        maxi_(other.itms_+n),
        YOCTO_DYNSTACK_MAKE()
        {
            YOCTO_DYNSTACK_CHECK();
            __copy(other);
            YOCTO_DYNSTACK_CHECK();
        }

        //! no-throw swap
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
        inline virtual
        ptrdiff_t height() const throw()
        {
            YOCTO_DYNSTACK_CHECK();
            return (static_cast<ptrdiff_t>(top_-head_));
        }

        //! total capacity
        inline virtual size_t capacity() const throw()
        {
            return maxi_;
        }

        //! total size
        inline virtual size_t size() const throw()
        {
            return itms_;
        }

        //! create a new frame
        inline void new_frame()
        {
            YOCTO_DYNSTACK_CHECK();
            this->save(static_cast<ptrdiff_t>(head_-addr_),
                       static_cast<ptrdiff_t>(tail_-addr_));
            head_ = top_;
            tail_ = head_-1;
            sub_  = head_-1;
        }

        inline void pop_frame() throw()
        {
            assert(hist.size>0);
            this->clear();
            // get frame
            frame_t *f = hist.pop_back();
            pool.store(f);
            head_ = addr_ + f->head;
            tail_ = addr_ + f->tail;
            sub_  = head_-1;
            top_  = tail_+1;
            YOCTO_DYNSTACK_CHECK();
        }



        //! reserve extra memory
        inline virtual void reserve(const size_t n)
        {
            if(n>0)
            {
                dynstack_of tmp(*this,n);
                swap_with(tmp);
                YOCTO_DYNSTACK_CHECK();
            }
        }


        //! push at top of current frame
        inline virtual
        void push( param_type arg )
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

        //! pop from current frame
        inline virtual
        void pop() throw()
        {
            assert(tail_>=head_);
            assert(height()>0);
            destruct<slot_type>(tail_);
            --tail_;
            --top_;
            --itms_;
        }


        //! free all frames
        inline virtual void free() throw()
        {
            __free_all();
        }


        //! free all frames dans release memory
        inline virtual void release() throw()
        {
            __release();
        }

        //! default name
        inline virtual const char *name() const throw() { return hidden::dynstack_name; }


        //! top of current frame
        inline virtual
        type &peek() throw()
        {
            assert(height()>0);
            return **tail_;
        }

        //! top of current frame, const
        inline virtual
        const_type &peek() const throw()
        {
            assert(height()>0);
            return **tail_;
        }

        //! base of current frame
        inline virtual
        type &base() throw()
        {
            assert(height()>0);
            return **head_;
        }

        //! base of current frame, const
        inline virtual
        const_type &base() const throw()
        {
            assert(height()>0);
            return **head_;
        }

        //! access
        /**
         - 1<=n<=height()   : base+n
         - -1>=n>=-heigth() : top+n
         */
        inline virtual
        type & operator[](ptrdiff_t n) throw()
        {
            if(n>0)
            {
                assert(n<=height());
                return *sub_[n];
            }
            else
            {
                assert(n<0);
                assert(n>=-height());
                return *top_[n];
            }
        }

        //! access, const
        inline virtual
        const_type & operator[](ptrdiff_t n) const throw()
        {
            if(n>0)
            {
                assert(n<=height());
                return *sub_[n];
            }
            else
            {
                assert(n<0);
                assert(n>=-height());
                return *top_[n];
            }
        }



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

