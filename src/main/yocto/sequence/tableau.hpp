#ifndef YOCTO_TABLEAU_INCLUDED
#define YOCTO_TABLEAU_INCLUDED 1


#include "yocto/sequence/array.hpp"
#include "yocto/bitwise.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{

    namespace hidden
    {
        extern const char tableau_name[];
    }
    

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


#define YOCTO_TABLEAU_IMPL(CODE) do { try { CODE; } catch(...){ release_();} } while(false)
#define YOCTO_TABLEAU_CTOR(COUNT) \
size_(0),        \
capacity_(COUNT),\
addr_(memory::kind<memory::global>::acquire_as<mutable_type>(capacity_) ),\
item_(addr_-1)

        //! ready to use 'zero' vector
        inline explicit tableau_of(size_t n) :
        YOCTO_TABLEAU_CTOR(n)
        {
            YOCTO_TABLEAU_IMPL( ctor0(int2type<is_primitive>(),n) );
        }


        //! vector full of 'value'
        inline explicit tableau_of(size_t n, param_type value ) :
        YOCTO_TABLEAU_CTOR(n)
        {
            YOCTO_TABLEAU_IMPL( ctor1(n,value) );
        }

        //! memory only
        inline explicit tableau_of(size_t n, const as_capacity_t & ) :
        YOCTO_TABLEAU_CTOR(n)
        {
        }

        inline explicit tableau_of(const tableau_of<T> &other ) :
        YOCTO_TABLEAU_CTOR(other.size_)
        {
            YOCTO_TABLEAU_IMPL( ctor1(other) );
        }

        inline explicit tableau_of(const tableau_of<T> &other, size_t n) :
        YOCTO_TABLEAU_CTOR(other.size_+n)
        {
            YOCTO_TABLEAU_IMPL( ctor1(other) );
        }


#define YOCTO_TABLEAU_XCH(FIELD) cswap_const(FIELD,other.FIELD)

        inline void swap_with( tableau_of<T> &other ) throw()
        {
            YOCTO_TABLEAU_XCH(size_);
            YOCTO_TABLEAU_XCH(capacity_);
            YOCTO_TABLEAU_XCH(addr_);
            YOCTO_TABLEAU_XCH(item_);
        }


        virtual void   free()     throw()       { free_();          }
        virtual void   release()  throw()       { release_();       }
        virtual size_t size()     const throw() { return size_;     }
        virtual size_t capacity() const throw() { return capacity_; }

        virtual const char * name() const throw() { return hidden::tableau_name; }

        virtual void reserve(size_t n)
        {
            if(n>0)
            {
                tableau_of<T> tmp(*this,n);
                swap_with(tmp);
            }
        }

        //! push back a new 'empty' object
        inline void push_back()
        {
            assert(size_<capacity_);
            new (addr_+size_) mutable_type();
            ++size_;
        }

        //! push_back by copy contructor
        inline void push_back(param_type value)
        {
            assert(size_<capacity_);
            new (addr_+size_) mutable_type(value);
            ++size_;
        }

        template <typename U>
        inline void append( typename type_traits<U>::parameter_type u )
        {
            assert(size_<capacity_);
            new (addr_+size_) mutable_type(u);
            ++size_;
        }

        template <typename U,typename V>
        inline void append(typename type_traits<U>::parameter_type u,
                           typename type_traits<V>::parameter_type v)
        {
            assert(size_<capacity_);
            new (addr_+size_) mutable_type(u,v);
            ++size_;
        }


    private:
        YOCTO_DISABLE_ASSIGN(tableau_of);
        size_t        size_;
        size_t        capacity_;
        mutable_type *addr_;
        mutable_type *item_;

        virtual  const_type *get_item() const throw() { return item_; }
        //______________________________________________________________________
        //
        // freeing stuff, using no destruct to optimize
        //______________________________________________________________________
        inline void free_( int2type<true> ) throw()
        {
            size_ = 0;
        }

        inline void free_( int2type<false> ) throw()
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
                ++size_;
            }
        }

        inline void ctor1( const size_t n, param_type value)
        {
            while(size_<n)
            {
                new (addr_+size_) mutable_type(value);
                ++size_;
            }
        }

        inline void ctor1(const tableau_of<T> &other)
        {
            assert(capacity_>=other.size_);
            while(size_<other.size_)
            {
                new (addr_+size_) mutable_type(other.addr_[size_]);
                ++size_;
            }
        }
        
        
        
    };
    
    
}

#endif
