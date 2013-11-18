#ifndef YOCTO_SEQUENCE_ARRAY_INCLUDED
#define YOCTO_SEQUENCE_ARRAY_INCLUDED 1

#include "yocto/container/sequence.hpp"
#include "yocto/container/iter-linear.hpp"
#include "yocto/memory/buffer.hpp"
#include <iostream>

namespace yocto
{
	
	namespace hidden
	{
		extern const char array_name[];
	}
	
	template <typename T>
	class array : public virtual container, public virtual memory::rw_buffer
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		virtual ~array() throw() {}
		
		//======================================================================
		// contiguous access
		//======================================================================
		inline type       & operator[]( size_t index ) throw()       { assert(index>0);assert(index<=size()); return *(type *)(get_item()+index); }
		inline const_type & operator[]( size_t index)  const throw() { assert(index>0);assert(index<=size()); return *(get_item()+index);         }
		
		inline type       * operator()( ptrdiff_t shift = 0 ) throw() { return item(1) + shift; }
		inline const_type * operator()( ptrdiff_t shift = 0 ) const throw() { return item(1) + shift; }

		//======================================================================
		// iterators
		//======================================================================
		typedef iterating::linear<type,iterating::forward> iterator;
		inline iterator begin() throw() { return iterator( item(1) );              }
		inline iterator end()   throw() { return iterator( item(this->size()+1) ); }
		
		typedef iterating::linear<const_type,iterating::forward> const_iterator;
		inline const_iterator begin() const throw() { return const_iterator( item(1) );              }
		inline const_iterator end()   const throw() { return const_iterator( item(this->size()+1) ); }
		
		typedef iterating::linear<type,iterating::reverse> reverse_iterator;
		inline reverse_iterator rbegin() throw() { return reverse_iterator( item(this->size()) ); }
		inline reverse_iterator rend()   throw() { return reverse_iterator( item(0) );     }
		
		typedef iterating::linear<const_type,iterating::reverse> const_reverse_iterator;
		inline const_reverse_iterator rbegin() const throw() { return const_reverse_iterator( item(this->size()) ); }
		inline const_reverse_iterator rend()   const throw() { return const_reverse_iterator( item(0) );     }
		
		inline friend std::ostream & operator<<( std::ostream &os, const array &a )
		{
			os << "[ ";
			for( size_t iao=1; iao <= a.size(); ++iao ) os <<  a[iao] << ' ';
			os << "]'";
			return os;
		}
        
        virtual size_t length() const throw() { return this->size() * sizeof(T); }
        
	protected:
		inline array() throw() {}
		
	private:
		virtual const_type *get_item() const throw() = 0; //!< for a 1..size access
		inline  type       *item(size_t index) throw()        { return (type *)get_item() + index; }
		inline  const_type *item(size_t index) const throw()  { return get_item()+index; }
        virtual const void *get_address() const throw() { return item(1); }
	};
	
}

#endif
