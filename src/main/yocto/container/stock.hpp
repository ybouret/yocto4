#ifndef YOCTO_STOCK_INCLUDED
#define YOCTO_STOCK_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/type/args.hpp"

namespace yocto {
	
	template <typename T>
	class stock : public container
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		virtual ~stock() throw() {}
		
		virtual void        push( param_type )    = 0;
		virtual const_type &peek() const throw()  = 0;
		virtual type       &peek() throw()        = 0;
		virtual void        pop() throw()         = 0;
		
		inline void pop( size_t n ) throw() 
		{
			assert( n <= this->size() );
			while( n-- > 0 ) this->pop();
		}
		
	protected:
		explicit stock() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(stock);
	};
	
}

#endif
