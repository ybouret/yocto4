#ifndef YOCTO_PIPE_INCLUDED
#define YOCTO_PIPE_INCLUDED 1

#include "yocto/container/stock.hpp"
#include <iostream>

namespace yocto {
	
	enum pipe_type
	{
		lifo_type,
		fifo_type
	};
	
	template <
	typename  T,
	typename  SEQUENCE,
	pipe_type TYPE
	>
	class pipe : public stock<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		explicit pipe() throw() : sequence_() {}
		virtual ~pipe() throw() {}
		explicit pipe( size_t n, const as_capacity_t &x ) : sequence_(n,x) {}
		
		//--------------------------------------------------------------
		//-- container
		//--------------------------------------------------------------
		
		virtual size_t size() const throw()     { return sequence_.size();      }
		virtual size_t capacity() const throw() { return sequence_.available(); }
		
		virtual void free() throw() { sequence_.free(); }
		virtual void release() throw() { sequence_.release(); }
		
		virtual void reserve( size_t n ) { sequence_.reserve(n); }
		
		//--------------------------------------------------------------
		//-- adaptor
		//--------------------------------------------------------------
		virtual void        push( param_type obj )  {   push( obj, int2type<TYPE>() ); }
		virtual void        pop() throw()           {         pop( int2type<TYPE>() ); }
		virtual const_type &peek() const throw()    { return peek( int2type<TYPE>() ); }
		virtual type       &peek() throw()          { return peek( int2type<TYPE>() ); }
        
        inline friend
        std::ostream & operator<<( std::ostream &os, const pipe &p)
        {
            os << p.sequence_;
            return os;
        }
        
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(pipe);
		
	protected:
		SEQUENCE sequence_;
		
	private:
		
		//--------------------------------------------------------------
		// LIFO -> think vector/stack
		//--------------------------------------------------------------
		inline void push( param_type obj ,int2type<lifo_type> )
		{
			sequence_.push_back(obj);
		}
		
		inline void pop( int2type<lifo_type> ) throw()
		{
			sequence_.pop_back();
		}
		
		inline const_type & peek( int2type<lifo_type> ) const throw()
		{
			return sequence_.back();
		}
		
		inline type & peek( int2type<lifo_type> )  throw()
		{
			return sequence_.back();
		}
		
		//--------------------------------------------------------------
		// FIFO -> think list/queue
		//--------------------------------------------------------------
		inline void push( param_type obj ,int2type<fifo_type> )
		{
			sequence_.push_back(obj);
		}
		
		inline void pop( int2type<fifo_type> ) throw()
		{
			sequence_.pop_front();
		}
		
		inline const_type & peek( int2type<fifo_type> ) const throw()
		{
			return sequence_.front();
		}
		
		inline type & peek( int2type<fifo_type> ) throw()
		{
			return sequence_.front();
		}
		
	};
	
	
}
#endif
