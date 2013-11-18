#ifndef YOCTO_HEAP_INCLUDED
#define YOCTO_HEAP_INCLUDED 1

#include "yocto/comparator.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/container/container.hpp"
#include "yocto/type/args.hpp"
#include "yocto/code/bswap.hpp"
#include <cstring>

//#include <iostream>

namespace yocto 
{
    
    
    namespace hidden
    {
        extern const char heap_name[];
    }
    
    //! heap of pointer to already existing objects
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
        
        //! insert the address
        inline void push( T *pObj )
        {
            if( size_ >= maxi_ )
                grow( next_increase(maxi_) );
            __push( pObj );
        }
        
        //! insert the address, no memory check
        inline void __push( T *pObj ) throw()
        {
            assert( size_ < maxi_ );
            //------------------------------------------------------------------
            //-- put it at the end
            //------------------------------------------------------------------
            size_t ipos = size_;
            size_t ppos = parent( ipos );
            data_[ size_++ ] = (mutable_type *)pObj;
            
            //------------------------------------------------------------------
            //-- organize heap
            //------------------------------------------------------------------
            while( ipos > 0 && comp_( *data_[ppos], *data_[ipos] ) < 0 )
            {
                cswap<slot_t>( data_[ppos], data_[ipos] );
                ipos = ppos;
                ppos = parent( ipos );
            }
            
        }
        
        //! return the highest data
        inline const_type & peek() const throw()
        {
            assert( size_ > 0 );
            assert( data_ > 0 );
            assert( data_[0] != NULL );
            return *data_[0];
        }
        
        
        //! remove the highest data
        inline T * pop() throw()
        {
            assert(size_>0);
            //------------------------------------------------------------------
            // promote the last element by replacing the top element
            //------------------------------------------------------------------
            T *ans = data_[0];
            data_[0] = data_[ --size_ ];
            
            //------------------------------------------------------------------
            // reorganize the heap from the top
            //------------------------------------------------------------------
            size_t ipos = 0;
            while( true )
            {
                const size_t lpos = at_left(  ipos );
                const size_t rpos = at_right( ipos );
                size_t       mpos = ipos;
                //--------------------------------------------------------------
                //-- test left
                //--------------------------------------------------------------
                if( lpos < size_ && comp_( *data_[ipos], *data_[lpos] ) < 0 )
                    mpos = lpos;
                
                //--------------------------------------------------------------
                //-- test right
                //--------------------------------------------------------------
                if( rpos < size_ && comp_( *data_[mpos], *data_[rpos] ) < 0 )
                    mpos = rpos;
                
                //--------------------------------------------------------------
                //-- are we done ?
                //--------------------------------------------------------------
                if( mpos == ipos )
                    break; 
                
                //--------------------------------------------------------------
                //-- update heap
                //--------------------------------------------------------------
                cswap<slot_t>( data_[ipos], data_[mpos] );
                ipos = mpos;
            }
            return ans;
        }
        
    private:
        typedef mutable_type *slot_t;
        ALLOCATOR      hmem_;
        size_t         size_;
        size_t         maxi_;
        slot_t        *data_;
        COMPARATOR     comp_;
        YOCTO_DISABLE_COPY_AND_ASSIGN(heap);
        
        static inline size_t parent( size_t npos ) throw()
        {
            return (npos-1) >> 1;
        }
        
        static inline size_t at_left( size_t npos ) throw()
        {
            return (npos<<1) + 1;
        }
        
        static inline size_t at_right( size_t npos ) throw()
        {
            return (npos<<1) + 2;
        }
        
        inline void kill() throw()
        { 
            size_ = 0;
            hmem_.template release_as<slot_t>( data_, maxi_ );
        }
        
        inline void grow( size_t n )
        {
            assert(n>0);
            //------------------------------------------------------------------
            //-- create new workspace
            //------------------------------------------------------------------
            size_t  new_maxi = maxi_ + n;
            slot_t *new_data = hmem_.template acquire_as<slot_t>( new_maxi );
            
            //------------------------------------------------------------------
            //-- transfer workspace
            //------------------------------------------------------------------
            memcpy( new_data, data_, sizeof(slot_t) * size_ );
            
            //------------------------------------------------------------------
            //-- remove old workspace
            //------------------------------------------------------------------
            hmem_.template release_as<slot_t>( data_, maxi_ );
            
            //------------------------------------------------------------------
            //-- reaffect data
            //------------------------------------------------------------------
            data_ = new_data;
            maxi_ = new_maxi;
        }
    };
    
}

#endif
