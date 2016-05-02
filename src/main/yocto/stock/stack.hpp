#ifndef YOCTO_STACK_INCLUDED
#define YOCTO_STACK_INCLUDED 1


#include "yocto/stock/pipe.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto {
	
	namespace hidden
	{
		extern const char stack_name[];
	}
	
	template<
	typename T,
	typename SEQUENCE = vector<T>
	>
	class stack : public pipe<T,SEQUENCE,lifo_type>
	{
public:
	YOCTO_ARGUMENTS_DECL_T;
	
	typedef pipe<T,SEQUENCE,lifo_type> pipe_type;
	
	explicit stack() throw() {}
	virtual ~stack() throw() {}
	explicit stack( size_t n, const as_capacity_t &x) : pipe_type(n,x)  {}
	
	virtual const char *name() const throw() { return hidden::stack_name; }
	
	type & operator[]( ptrdiff_t n ) throw()
	{
		return pipe_type::sequence_[ get_stack_offset(n) ];
	}
	
	const_type & operator[]( ptrdiff_t n ) const throw()
	{
		return pipe_type::sequence_[ get_stack_offset(n) ];
	}
	
private:
	YOCTO_DISABLE_COPY_AND_ASSIGN(stack);
	size_t get_stack_offset( ptrdiff_t n ) const throw()
	{
		assert( static_cast<size_t>( n < 0 ? -n : n ) <= this->size() );
		if( n > 0 )
		{
			return static_cast<size_t>(n);
		}
		else
		{
			size_t   m = pipe_type::sequence_.size();
			return ++m - static_cast<size_t>(-n);
		}
	}
	
};


}


#endif
