#include "yocto/rx/source.hpp"

namespace yocto 
{
	namespace regex
	{
		
		
		source:: ~source() throw()
		{
		}
		
		source:: source() throw() : char_pool(), cache_(), input_(NULL)
		{
		}
		
		void source:: reset() throw()
		{
			cache_.back_to(char_pool);
		}
		
		void source:: connect( ios::istream &input ) throw()
		{
			reset();
			input_ = &input;
		}
		
		void source:: disconnect() throw()
		{
			reset();
			input_ = NULL;
		}
		
		t_char * source:: get()
		{
			assert( input_ );
			if( cache_.size > 0 )
			{
				return cache_.pop_front();
			}
			else 
			{
				t_char *ch = char_pool.acquire();
				try 
				{
					if( input_->query( ch->data ) )
					{
						
						return ch;
					}
					else
					{
						char_pool.store( ch );
						return NULL;
					}
					
				}
				catch (...) {
					char_pool.store(ch);
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
			token  cpy( tkn, char_pool );
			unget( cpy );
		}
		
		
		bool source:: cache1()
		{
			t_char *ch = char_pool.acquire();
			try {
				
				if( !input_->query( ch->data ) )
				{
					char_pool.store( ch );
					return false;
				}
				else {
					cache_.push_back( ch );
					return true;
				}
				
			}
			catch (...) {
				char_pool.store( ch );
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
		
		void source:: drop( t_char *ch ) throw() { char_pool.store( ch );  }
		void source:: drop( token &tkn ) throw() { tkn.back_to(char_pool); }
		
	
		
		void source:: skip(size_t n) throw()
		{
			assert( n <= in_cache() );
			while(n-->0) char_pool.store( cache_.pop_front() );
		}
	}
}
