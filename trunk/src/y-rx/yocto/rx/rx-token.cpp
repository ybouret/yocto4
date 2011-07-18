#include "yocto/rx/token.hpp"
#include "yocto/object.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		t_char * t_char:: acquire() 
		{
			t_char *ch = object::acquire1<t_char>();
			assert( NULL == ch->prev );
			assert( NULL == ch->next );
			return ch;
		}
		
		void t_char:: release( t_char *ch ) throw()
		{
			assert(ch);
			assert( NULL == ch->prev );
			assert( NULL == ch->next );
			object::release1<t_char>(ch);
		}
		
		t_pool:: t_pool() throw() : core::pool_of<t_char>()
		{
		}
		
		t_pool:: ~t_pool() throw()
		{
			release();
		}
		
		t_char * t_pool:: create( char c ) 
		{
			t_char *ch = size > 0 ? query() : t_char::acquire();
			ch->data = c;
			return ch;
		}
		
		void t_pool:: release() throw()
		{
			delete_with( t_char::release );
		}
		
		void t_pool:: reserve( size_t n )
		{
			while( n-- > 0 ) store( t_char::acquire() );
		}
		
		
		token::  token() throw() {}
		token:: ~token() throw() { release(); }
		
		void token:: release() throw()
		{
			 delete_with( t_char::release );
		}
		
		void token:: back_to( t_pool &pool ) throw()
		{
			while( size ) pool.store( pop_back() );
		}
		
		token:: token( const token &other ) 
		{
			try 
			{
				for( const t_char *ch = other.head; ch; ch=ch->next )
				{
					t_char *C = t_char::acquire();
					C->data   = ch->data;
					push_back( C );
				}
			}
			catch(...)
			{
				release();
				throw;
			}
		}
		
		token:: token( const token &other, t_pool &pool ) 
		{
			try 
			{
				for( const t_char *ch = other.head; ch; ch=ch->next )
				{
					t_char *C = pool.create( ch->data );
					push_back( C );
				}
			}
			catch(...)
			{
				back_to(pool);
				throw;
			}
		}
		
		void token:: at_head( token &other ) throw()
		{
			while( size ) other.push_front( pop_back() );
		}
		
	}
	
}
