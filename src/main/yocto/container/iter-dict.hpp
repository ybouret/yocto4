#ifndef YOCTO_CONTAINER_DICT_ITER_INCLUDED
#define YOCTO_CONTAINER_DICT_ITER_INCLUDED 1

#include "yocto/container/iterator.hpp"
#include "yocto/type/args.hpp"

namespace yocto
{
	
	namespace iterating
	{
		
		template <typename T,typename SLOT,direction D>
		class dict
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			inline dict() throw()       : item_(NULL) {}
			inline dict( SLOT **p ) throw() : item_( p )  {}
			inline dict( const dict &other) throw() : item_(other.item_) {}
			inline dict & operator=( const dict &other ) throw() { item_ = other.item_; return *this; }
			
			inline dict & operator++() throw() { this->increase( int2type<D>() );  return *this; }
			inline dict & operator--() throw() { this->decrease( int2type<D>() );  return *this; }
			
			inline dict & operator+=( ptrdiff_t n ) throw()  { this->add( n, int2type<D>() ); return *this; }
			inline dict   operator+( ptrdiff_t n ) const throw() { dict tmp( *this ); return tmp += n; }
			
			inline dict & operator-=( ptrdiff_t n ) throw()  { this->sub( n, int2type<D>() ); return *this; }
			inline dict   operator-( ptrdiff_t n ) const throw() { dict tmp( *this ); return tmp -= n; }
			
			inline type       & operator*() throw()       { assert(item_); assert(*item_); return (*item_)->data; }
			inline const_type & operator*() const throw() { assert(item_); assert(*item_); return (*item_)->data; }
			
			inline friend bool operator==( const dict &lhs, const dict &rhs ) throw() { return lhs.item_ == rhs.item_; }
			inline friend bool operator!=( const dict &lhs, const dict &rhs ) throw() { return lhs.item_ != rhs.item_; }
			

			SLOT * operator->() throw() { assert(item_); assert(*item_); return (*item_); }

		private:
			SLOT **item_;
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
