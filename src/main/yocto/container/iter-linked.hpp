#ifndef YOCTO_CONTAINER_LINKED_ITER_INCLUDED
#define YOCTO_CONTAINER_LINKED_ITER_INCLUDED 1

#include "yocto/container/iterator.hpp"
#include "yocto/type/args.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
	
	namespace iterating
	{
		
		template <typename T, typename NODE, direction D>
		class linked
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			inline linked() throw()       : node_(NULL) {}
			inline linked( NODE *p ) throw() : node_( p )  {}
			inline linked( const linked &other) throw() : node_(other.node_) {}
			inline linked & operator=( const linked &other ) throw() { node_ = other.node_; return *this; }
			
			inline linked & operator++() throw() { this->increase( int2type<D>() );  return *this; }
			inline linked & operator--() throw() { this->decrease( int2type<D>() );  return *this; }
			
			inline linked & operator+=( ptrdiff_t n ) throw()  { this->add( n, int2type<D>() ); return *this; }
			inline linked   operator+( ptrdiff_t n ) const throw() { linked tmp( *this ); return tmp += n; }
			
			inline linked & operator-=( ptrdiff_t n ) throw()  { this->sub( n, int2type<D>() ); return *this; }
			inline linked   operator-( ptrdiff_t n ) const throw() { linked tmp( *this ); return tmp -= n; }
			
			inline type       & operator*() throw()       { assert(node_); return node_->data; }
			inline const_type & operator*() const throw() { assert(node_); return node_->data; }
						
			inline friend bool operator==( const linked &lhs, const linked &rhs ) throw() { return lhs.node_ == rhs.node_; }
			inline friend bool operator!=( const linked &lhs, const linked &rhs ) throw() { return lhs.node_ != rhs.node_; }
			
			NODE * operator->() throw(){ assert(node_); return node_; }
			
		private:
			NODE *node_;
			
			//-- forward
			inline void increase( int2type<forward> ) throw() { node_=node_->next; }
			inline void decrease( int2type<forward> ) throw() { node_=node_->prev; }
			inline void add( ptrdiff_t n, int2type<forward> ) throw() { if(n>=0) YOCTO_LOOP(n,node_=node_->next); else YOCTO_LOOP_(-n,node_=node_->prev); }
			inline void sub( ptrdiff_t n, int2type<forward> ) throw() { if(n>=0) YOCTO_LOOP(n,node_=node_->prev); else YOCTO_LOOP_(-n,node_=node_->next); }
			
			//-- reverse
			inline void increase( int2type<reverse> ) throw() { node_=node_->prev; }
			inline void decrease( int2type<reverse> ) throw() { node_=node_->next; }
			inline void add( ptrdiff_t n, int2type<reverse> ) throw() { if(n>=0) YOCTO_LOOP(n,node_=node_->prev); else YOCTO_LOOP_(-n,node_=node_->next); }
			inline void sub( ptrdiff_t n, int2type<reverse> ) throw() { if(n>=0) YOCTO_LOOP(n,node_=node_->next); else YOCTO_LOOP_(-n,node_=node_->prev); }
			
			
		};
	}
	
}


#endif
