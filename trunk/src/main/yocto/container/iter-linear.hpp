#ifndef YOCTO_CONTAINER_LINEAR_ITER_INCLUDED
#define YOCTO_CONTAINER_LINEAR_ITER_INCLUDED 1

#include "yocto/container/iterator.hpp"
#include "yocto/type/args.hpp"

namespace yocto
{
	
	namespace iterating
	{
		
		template <typename T,direction D>
		class linear
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			inline linear() throw()       : item_(NULL) {}
			inline linear( T *p ) throw() : item_((mutable_type*)p )  {}
			inline linear( const linear &other) throw() : item_(other.item_) {}
			inline linear & operator=( const linear &other ) throw() { item_ = other.item_; return *this; }
			
			inline linear & operator++() throw() { this->increase( int2type<D>() );  return *this; }
			inline linear & operator--() throw() { this->decrease( int2type<D>() );  return *this; }
			
			inline linear & operator+=( ptrdiff_t n ) throw()  { this->add( n, int2type<D>() ); return *this; }
			inline linear   operator+( ptrdiff_t n ) const throw() { linear tmp( *this ); return tmp += n; }
			
			inline linear & operator-=( ptrdiff_t n ) throw()  { this->sub( n, int2type<D>() ); return *this; }
			inline linear   operator-( ptrdiff_t n ) const throw() { linear tmp( *this ); return tmp -= n; }
			
			inline type       & operator*() throw()       { assert(item_); return *item_; }
			inline const_type & operator*() const throw() { assert(item_); return *item_; }
			
			inline friend bool operator==( const linear &lhs, const linear &rhs ) throw() { return lhs.item_ == rhs.item_; }
			inline friend bool operator!=( const linear &lhs, const linear &rhs ) throw() { return lhs.item_ != rhs.item_; }
			
		private:
			mutable_type *item_;
			//-- forward
			inline void increase( int2type<forward> ) throw() { ++item_; }
			inline void decrease( int2type<forward> ) throw() { --item_; }
			inline void add( ptrdiff_t n, int2type<forward> ) throw() { item_ += n; }
			inline void sub( ptrdiff_t n, int2type<forward> ) throw() { item_ -= n; }
			
			//-- reverse
			inline void increase( int2type<reverse> ) throw() { --item_; }
			inline void decrease( int2type<reverse> ) throw() { ++item_; }
			inline void add( ptrdiff_t n, int2type<reverse> ) throw() { item_ -= n; }
			inline void sub( ptrdiff_t n, int2type<reverse> ) throw() { item_ += n; }
			
			
		};
	}
	
}


#endif
