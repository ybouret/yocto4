#include "yocto/fragment/block.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/utils.hpp"

#include <cstring>

namespace yocto
{
	
	namespace fragment
	{
		
		block:: ~block() throw()
		{
			assert(next==NULL);
			assert(prev==NULL);
			memory::kind<memory::global>::release_as<uint8_t>( first, (size_t&)bytes );
			final = NULL;
			curr  = NULL;
			last  = NULL;
		}
		
#define YFRAG_CHECK(B)                           \
assert( (B)->first + (B)->bytes == (B)->final ); \
assert( (B)->curr >= (B)->first );               \
assert( (B)->last >= (B)->curr  );               \
assert( (B)->last <= (B)->final )
		
		void block:: clear() throw()
		{
			YFRAG_CHECK(this);
			curr = last = first;
			YFRAG_CHECK(this);
		}
		
		
		block:: block( size_t len ) :
		next(NULL),
		prev(NULL),
		bytes( len ),
		first( memory::kind<memory::global>::acquire_as<uint8_t>( (size_t&)bytes ) ),
		final( first + bytes ),
		curr( first ),
		last( first )
		{
			YFRAG_CHECK(this);
		}
		
		size_t block:: length() const throw()
		{
			YFRAG_CHECK(this);
			return static_cast<size_t>(last-curr);
		}
		
		size_t block:: unused() const throw()
		{
			YFRAG_CHECK(this);
			return static_cast<size_t>(final-last);
		}
		
		size_t block:: offset() const throw()
		{
			YFRAG_CHECK(this);
			return static_cast<size_t>(curr-first);
		}
		
		size_t block:: write( const void *buffer, size_t buflen ) throw()
		{
			assert( !(buffer==NULL&&buflen>0) );
			YFRAG_CHECK(this);
			const size_t len = min_of(buflen,unused());
			memcpy(last,buffer,len);
			last += len;
			YFRAG_CHECK(this);
			return len;
		}
		
		size_t block:: read( void *buffer, size_t buflen ) throw()
		{
			assert( !(buffer==NULL&&buflen>0) );
			YFRAG_CHECK(this);
			if( buflen < length() )
			{
				memcpy( buffer, curr, buflen );
				curr += buflen;
				YFRAG_CHECK(this);
				return buflen;
			}
			else 
			{
				const size_t len = length();
				memcpy( buffer, curr, len );
				clear();
				YFRAG_CHECK(this);
				return len;
			}
		}
		
		void block:: defrag() throw()
		{
			YFRAG_CHECK(this);
			const size_t len = length();
			memmove( first, curr, len );
			curr = first;
			last = curr + len;
			YFRAG_CHECK(this);
		}
		
		bool   block:: try_steal( block &other ) throw()
		{
			assert( this != &other );
			YFRAG_CHECK(this);
			YFRAG_CHECK(&other);
			const size_t len = other.length();
			if( len <= unused() )
			{
				memcpy( last, other.curr, len );
				last += len;
				YFRAG_CHECK(this);
				other.clear();
				return true;
			}
			else 
				return false;
		}
		
		bool block:: back( uint8_t x ) throw()
		{
			YFRAG_CHECK(this);
			if( curr > first )
			{
				*(--curr) = x;
				YFRAG_CHECK(this);
				return true;
			}
			else 
			{
				return false;
			}
			
		}
		
		bool block:: recv( ios::ichannel &input )
		{
			YFRAG_CHECK(this);
			size_t nr = 0;
			input.get( last, unused(), nr );
			assert(nr<=unused());
			last  += nr;
			YFRAG_CHECK(this);
			return nr > 0 ;
		}
		
	}
	
}
