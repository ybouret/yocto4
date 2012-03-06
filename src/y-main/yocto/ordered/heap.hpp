#ifndef YOCTO_HEAP_INCLUDED
#define YOCTO_HEAP_INCLUDED 1

#include "yocto/comparator.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/container/container.hpp"
#include "yocto/type-traits.hpp"
#include <cstring>

namespace yocto 
{
    
    
    namespace hidden
    {
        extern const char heap_name[];
    }
    
    template <
	typename T,
	typename COMPARATOR = comparator<T>,
	typename ALLOCATOR  = memory::global::allocator
	>
	class heap : public container
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        explicit heap() throw() : hmem_(), size_(0), maxi_(0), data_(NULL), comp_() {}
        explicit heap( size_t n, const  as_capacity_t & ) :
        hmem_(),
        size_(0),
        maxi_(n),
        data_( hmem_.template acquire_as<slot_t>( maxi_ ) ),
        comp_()
        {
        }
        
        virtual ~heap() throw() { kill(); }
        
        virtual size_t size()     const throw() { return size_; }
        virtual size_t capacity() const throw() { return maxi_; }
        virtual const char *name() const throw() { return hidden::heap_name; }
        
        virtual void free()    throw()   { size_ = 0; }
        virtual void release() throw()   { kill();    }
        virtual void reserve( size_t n ) { if( n > 0 ) grow(n);}
        
        void push( param_type obj )
        {
            //-- check memory
            if( size_ >= maxi_ ) grow( next_increase(maxi_) );
            //-- get the address of obj
            mutable_type *args = (mutable_type *)&obj;
            
            //-- put it at the end
            data_[ size_++ ] = args;
        }
        
    private:
        typedef mutable_type *slot_t;
        ALLOCATOR      hmem_;
        size_t         size_;
        size_t         maxi_;
        slot_t        *data_;
        COMPARATOR     comp_;
        YOCTO_DISABLE_COPY_AND_ASSIGN(heap);
        inline void kill() throw()
        { 
            size_ = 0;
            hmem_.template release_as<slot_t>( data_, maxi_ );
        }
        
        inline void grow( size_t n )
        {
            assert(n>0);
            //-- create new workspace
            size_t  new_maxi = maxi_ + n;
            slot_t *new_data = hmem_.template acquire_as<slot_t>( new_maxi );
            //-- transfer workspace
            memcpy( new_data, data_, sizeof(slot_t) * size_ );
            
            //-- remove old workspace
            hmem_.template release_as<slot_t>( data_, maxi_ );
            
            //-- reaffect data
            data_ = new_data;
            maxi_ = new_maxi;
        }
    };
    
}

#endif
