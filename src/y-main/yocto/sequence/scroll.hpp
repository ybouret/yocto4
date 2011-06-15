#ifndef YOCTO_SCROLL_INCLUDED
#define YOCTO_SCROLL_INCLUDED 1

#include "yocto/sequence/list.hpp"

namespace yocto
{
	
	namespace hidden
	{
		extern const char scroll_name[];
	}
	
	template <typename T>
	class scroll : public list<T>
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		explicit scroll() throw() : list<T>() {}
		explicit scroll( size_t n, const as_capacity_t & ) : list<T>( n, as_capacity ) {}
		virtual ~scroll() throw() {}
		scroll( const scroll &other ) : list<T>( other ) {}
		virtual const char * name() const throw() { return hidden::scroll_name; }
		
		inline void at_next( param_type args )
		{
			assert( this->size() > 0 );
			list<T>::list_.push_back( replace( list<T>::list_.pop_front(), args ) );
		}
		
		inline void at_prev( param_type args )
		{
			assert( this->size() > 0 );
			list<T>::list_.push_front( replace( list<T>::list_.pop_back(), args ) );
		}
		
		
	private:
		YOCTO_DISABLE_ASSIGN(scroll);
		inline  
		typename list<T>::node_type *
		replace( typename list<T>::node_type *node, param_type args )
		{
			destruct( & node->data );
			try
			{
				new ( &node->data) mutable_type( args );
				return node;
			}
			catch(...)
			{
				list<T>::pool_.store( node );
				throw;
			}
		}
		
	};
	
	
	template <typename T>
	class autoscroll : public scroll<T>
	{
	public:
		explicit autoscroll( size_t w ) throw() : scroll<T>( checkw_(w), as_capacity ), width( checkw_(w) ) { assert(width>0); }
		virtual ~autoscroll() throw() {}
		autoscroll( const autoscroll &other ) : scroll<T>( other ), width( other.width ) {}
		
		void set_width( size_t w )
		{
			assert( width > 0 );
			w = checkw_(w);
			if( w <= width )
			{
				update_( w );
				while( this->size() > width )
					this->pop_front();
			}
			else
			{
				// w > width
				this->reserve( w - width );
				update_(w);
			}
		}
		
		void record( typename scroll<T>::param_type args )
		{
			if( this->size() >= width )
				this->at_next( args );
			else
				this->push_back( args );
		}
		
		const size_t width;
		
	private:
		YOCTO_DISABLE_ASSIGN(autoscroll);
		inline void          update_( size_t w ) throw() { *(size_t *) & width = w;  }
		inline static size_t checkw_( size_t w ) throw() { return  w > 1 ? w : 1;    }
	};
	
}

#endif
