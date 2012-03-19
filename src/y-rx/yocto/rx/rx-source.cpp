#include "yocto/rx/source.hpp"

namespace yocto 
{
	namespace regex
	{
		
		
		source:: ~source() throw()
		{
		}
		
		source:: source( ios::istream &input ) throw() : cache_(), input_(input)
		{
		}
		
				
		t_char * source:: get()
		{
			if( cache_.size > 0 )
			{
				return cache_.pop_front();
			}
			else 
			{
				t_char *ch = t_char::acquire();
				try 
				{
					if( input_.query( ch->data ) )
					{
						
						return ch;
					}
					else
					{
						t_char::release(ch);
						return NULL;
					}
					
				}
				catch (...) {
					t_char::release(ch);
					throw;
				}
			}
		}
		
		void source:: unget( t_char *ch ) throw()
		{
			assert( ch );
			cache_.push_front( ch );
		}
		
		void source:: unget( token &tkn ) throw()
		{
			tkn.move_at_head_of( cache_ );
		}
		
		void source:: uncpy( const token &tkn )
		{
			token  cpy( tkn );
			unget( cpy );
		}
		
		
		bool source:: cache1()
		{
			t_char *ch = t_char::acquire();
			try {
				
				if( !input_.query( ch->data ) )
				{
					t_char::release( ch );
					return false;
				}
				else {
					cache_.push_back( ch );
					return true;
				}
				
			}
			catch (...) {
				t_char::release( ch );
				throw;
			}
			
		}
		
		const t_char  * source:: peek()
		{
			if( cache_.size <= 0 && ! cache1() )
				return NULL;
			return cache_.head;
		}
		
		void   source:: prefetch(size_t n)
		{
			while( n-- > 0 )
			{
				if( !cache1() ) return;
			}
		}
		
		size_t source:: in_cache() const throw()
		{
			return cache_.size;
		}
		
		
		
		void source:: skip(size_t n) throw()
		{
			assert( n <= in_cache() );
			while(n-->0) t_char::release( cache_.pop_front() );
		}
		
		bool source:: is_active() 
		{
			 return NULL != peek();
		}
	}
}
