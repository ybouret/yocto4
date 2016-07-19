#ifndef YOCTO_SOFT_PTR_INCLUDED
#define YOCTO_SOFT_PTR_INCLUDED 1


#include "yocto/type/args.hpp"
#include "yocto/ptr/ref.hpp"

namespace yocto
{

    template <typename T>
    class soft_ptr
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        inline soft_ptr( T *p ) : pointee_( (mutable_type*)p ), nref_(NULL)
        {
            if(pointee_)
            {
                try { nref_ = core::create_ref(); }
                catch(...) { delete p; throw; }
                assert( nref_);
                assert( 1 == *nref_ );
            }

        }

        inline ~soft_ptr() throw()
        {
            if(pointee_)
            {
                assert( nref_!=0);
                assert(*nref_> 0);

                if( --(*nref_) <= 0 )
                {
                    delete pointee_;
                    pointee_ = NULL;
                    core::delete_ref( nref_ );
                    assert( NULL == nref_ );
                }
            }
        }

        inline soft_ptr( const soft_ptr &other ) throw() :
        pointee_( other.pointee_ ),
        nref_(    other.nref_    )
        {
            if(pointee_)
            {
                assert( nref_!=0);
                assert(*nref_> 0);
                ++(*nref_);
            }
        }

        inline bool is_valid() const throw() { return 0 != pointee_; }

        inline type & operator*()  throw()
        {
            assert(is_valid());
            return *pointee_;
        }

        inline const_type & operator*()  const throw()
        {
            assert(is_valid());
            return *pointee_;
        }

        inline type * operator->() throw()
        {
            assert(is_valid());
            return pointee_;
        }

        inline const_type * operator->() const throw()
        {
            assert(is_valid());
            return pointee_;
        }

        inline type       * __get() throw()       { return pointee_; }
        inline const_type * __get() const throw() { return pointee_; }


    private:
        mutable_type *pointee_;
        size_t       *nref_;
        YOCTO_DISABLE_ASSIGN(soft_ptr);
    };
    
}

#endif
