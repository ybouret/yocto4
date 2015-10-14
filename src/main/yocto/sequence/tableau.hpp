#ifndef YOCTO_TABLEAU_INCLUDED
#define YOCTO_TABLEAU_INCLUDED 1


#include "yocto/sequence/array.hpp"
#include "yocto/bitwise.hpp"

namespace yocto
{

    template <typename T>
    class tableau_of : public array<T>
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        enum
        {
            is_primitive = type_traits<mutable_type>::is_primitive //|| support_c_style_ops<mutable_type>::value
        };

        virtual ~tableau_of() throw()
        {
            release_();
        }

        explicit tableau_of() throw() :
        size_(0),
        capacity_(0),
        addr_(0),
        item_(addr_-1)
        {
        }


#define YOCTO_TABLEAU_CTOR(CODE) do { try { CODE; } catch(...){ release_();} } while(false)

        explicit tableau_of(size_t n) :
        size_(0),
        capacity_(n),
        addr_( memory::kind<memory::global>::acquire_as<mutable_type>(capacity_) ),
        item_( addr_-1 )
        {
            YOCTO_TABLEAU_CTOR(ctor0( int2type<is_primitive>(), n ) );
        }



        virtual void   free()     throw()       { free_();          }
        virtual void   release()  throw()       { release_();       }
        virtual size_t size()     const throw() { return size_;     }
        virtual size_t capacity() const throw() { return capacity_; }

        virtual const char * name() const throw() { return "tableau"; }
        virtual void reserve(size_t n)
        {

        }

    private:
        size_t        size_;
        size_t        capacity_;
        mutable_type *addr_;
        mutable_type *item_;

        virtual  const_type *get_item() const throw() { return item_; }
        //______________________________________________________________________
        //
        // freeing stuff, using no destruct to optimize
        //______________________________________________________________________
        inline void free_( int2type<false> ) throw()
        {
            size_ = 0;
        }

        inline void free_( int2type<true> ) throw()
        {
            while(size_>0)
            {
                destruct(&addr_[--size_]);
            }
        }

        inline void free_() throw()
        {
            free_( int2type< support_no_destruct<mutable_type>::value >() );
        }

        //______________________________________________________________________
        //
        // release all memory
        //______________________________________________________________________
        inline void release_() throw()
        {
            free_();
            memory::kind<memory::global>::release_as<mutable_type>(addr_,capacity_);
            item_ = addr_-1;
        }

        //______________________________________________________________________
        //
        // constructors, using is_primitive as router
        //______________________________________________________________________
        inline void ctor0( int2type<true>, const size_t n) throw()
        {
            size_ = n;
        }

        inline void ctor0( int2type<false>, const size_t n) throw()
        {
            while(size_<n)
            {
                new (addr_+size_) mutable_type();
            }
        }

    };


}

#endif
