//! records.hpp
/**
 C-Style records of C++ objects
 */

#ifndef YOCTO_RECORDS_INCLUDED
#define YOCTO_RECORDS_INCLUDED 1

#include "yocto/type-traits.hpp"

namespace yocto
{
	
    namespace memory
    {
		
		//! smart small vector
		/**
		 
		 */
        template <typename T>
        class records_of
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;

			explicit records_of() throw() : size_(0), maxi_(0), base_(NULL) {}
			
            explicit records_of( T *p, size_t n ) throw() :
			size_( 0 ),
			maxi_( n ),
			base_( (mutable_type *) p ) {
            }
			
			
            virtual ~records_of() throw() {
                free();
            }
			
            void append( param_type args ) {
                assert( size_ < maxi_ );
                new ( base_ + size_ ) mutable_type( args );
                ++size_;
            }
			
            inline type & operator[]( size_t index ) throw() {
                assert( index < size_ );
                return base_[ index ];
            }
			
            inline const_type & operator[]( size_t index ) const throw() {
                assert( index < size_ );
                return base_[ index ];
            }
			
            inline type       * operator()(ptrdiff_t shift) throw()       {
                return base_+shift;
            }
            inline const_type * operator()(ptrdiff_t shift) const throw() {
                return base_+shift;
            }
			
            inline void rebase( T *p, size_t n ) throw() {
                free();
                maxi_ = n;
                base_ = p;
            }
			
            inline void free() throw() {
                while( size_ > 0 )
                    base_[ --size_ ].~mutable_type();
            }
			
			inline size_t size() const throw()      { return size_; }
			inline size_t capacity() const throw()  { return maxi_; }
			inline size_t available() const throw() { return maxi_ - size_; } 
			
        private:
			size_t        size_;
			size_t        maxi_;
            mutable_type *base_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(records_of);
            			
        };
		
    }
	
}

#endif
