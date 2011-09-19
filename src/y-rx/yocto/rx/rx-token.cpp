#include "yocto/rx/token.hpp"
#include "yocto/object.hpp"
#include <iostream>

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
			t_char *ch = acquire();
			ch->data = c;
			return ch;
		}
		
		t_char * t_pool:: acquire()
		{
			return size > 0 ? query() : t_char::acquire();
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
		
		void token:: move_at_head_of( token &other ) throw()
		{
			while( size ) other.push_front( pop_back() );
		}
		
		void token:: move_at_tail_of( token &other ) throw()
		{
			while( size ) other.push_back( pop_front() );
		}
		
		string token:: to_string( size_t skip, size_t trim) const
		{
			const size_t nz = skip+trim;
			assert( nz <= size );
			const t_char *ch = head;
			while( skip-- > 0 ) ch = ch->next;
			size_t        ns = size - nz;
			string ans(ns,as_capacity);
			while( ns-- > 0 ) { ans.append( ch->data ); ch = ch->next; }
			return ans;
		}
		
		std::ostream & operator<< ( std::ostream &os, const token &tkn )
		{
			for( const t_char *ch = tkn.head; ch; ch=ch->next)
			{
				char C = ch->data;
				if( C < 32 || C >= 127 ) C = '.';
				os << C;
			}
			return os;
		}

		
	}
	
}
