#ifndef YOCTO_ALIAS_PTR_INCLUDED
#define YOCTO_ALIAS_PTR_INCLUDED 1


#include "yocto/type/key.hpp"

namespace yocto
{

    //! alias pointer, no operations on pointee
    template <typename KEY,typename T>
    class alias_ptr
    {
    public:
        YOCTO_ASSOCIATIVE_KEY_T;

        inline alias_ptr( T *p ) : pointee_( (mutable_type*)p )
        {

        }

        inline ~alias_ptr() throw() {}
        

        inline alias_ptr( const alias_ptr &other ) throw() :
        pointee_( other.pointee_ )
        {
        }


        inline type & operator*()  throw()
        {
            assert(pointee_);
            return *pointee_;
        }

        inline const_type & operator*()  const throw()
        {
            assert(pointee_);
            return *pointee_;
        }

        inline type * operator->() throw()
        {
            assert(pointee_);
            return pointee_;
        }

        inline const_type * operator->() const throw()
        {
            assert(pointee_);
            return pointee_;
        }

        inline type       * __get() throw()       { return pointee_; }
        inline const_type * __get() const throw() { return pointee_; }

        inline const_key & key() const throw() { return pointee_->key(); }
        
        inline void swap_with( alias_ptr &other ) throw()
        {
            mutable_type *tmp = other.pointee_;
            other.pointee_    = pointee_;
            pointee_          = tmp;
        }

    private:
        mutable_type *pointee_;
        YOCTO_DISABLE_ASSIGN(alias_ptr);
    };
    
}

#endif
