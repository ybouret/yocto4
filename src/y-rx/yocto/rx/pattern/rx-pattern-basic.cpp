#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	namespace regex
	{
		namespace basic
		{
			
			one_char:: one_char( uint32_t t ) throw() : pattern( t ) {}
			one_char:: ~one_char() throw() {}
			
			bool one_char:: accept( source &src )
			{
				assert( 0 == size );
				t_char *ch = src.get();
				if( ch )
				{
					if( is_valid(ch->data) )
					{
						push_back(ch);
						return true;
					}
					else 
					{
						src.unget(ch);
						return false;
					}
					
				}
				else 
				{
					//! End Of Source
					return false;
				}
				
			}
			
			
			////////////////////////////////////////////////////////////////////
			any1 *  any1:: create() { return new any1(); }
			any1::  any1() throw() : one_char(id) {}
			any1:: ~any1() throw() {}
			bool    any1:: is_valid( char ) const throw() { return true; }
			
			////////////////////////////////////////////////////////////////////
			single *  single:: create(char c) { return new single(c); }
			single::  single(char c) throw() : one_char(id),value(c) { data = (void*)&value; }
			single:: ~single() throw() {}
			bool      single:: is_valid( char c ) const throw() { return c == value; }
			
			////////////////////////////////////////////////////////////////////
			range *  range:: create(char a,char b) 
			{
				const uint8_t A = a;
				const uint8_t B = b;
				if( A <= B )
					return new range(A,B);
				else {
					return new range(B,A);
				}
				
			}
			range::  range(uint8_t A,uint8_t B) throw() : one_char(id),lower(A),upper(B) {  assert(A<=B); }
			range:: ~range() throw() {}
			bool      range:: is_valid( char c ) const throw()
			{ 
				const uint8_t C = c;
				return C >= lower && C <= upper;
			}
			
			////////////////////////////////////////////////////////////////////
			within * within:: create()
			{
				return new within();
			}
					
			within:: within() throw() : one_char(id), impl()
			{
				data = &impl;
			}
			
			within:: within( const within &w ) : one_char(id), impl( w.impl ) 
			{
				data = &impl;
			}
			
			within:: ~within() throw() {}
			
			bool within:: is_valid( char c ) const throw() { return impl.search( uint8_t(c) ); }
			
			void within::add( char x )
			{
				(void) impl.insert( uint8_t(x) );
			}
			
			void within:: add( char a, char b )
			{
				uint8_t A = a;
				uint8_t B = b;
				if( A > B )
				{
					cswap(A,B);
				}
				for( size_t i=A; i <= size_t(B); ++i )
				{
					(void) impl.insert( uint8_t(i) );
				}
			}
			
		}
	}
}
