#include "yocto/net/io/block.hpp"
#include "yocto/memory/global.hpp"

#include <cstring>

namespace yocto
{
	namespace network
	{
		
		io_block:: ~io_block() throw()
		{
			size_t n = final - start; assert(n>0);
			memory::kind<memory::global>::release_as<uint8_t>( start, n );
			final = NULL;
			curr  = last = NULL;
		}
		
		io_block:: io_block( size_t bs ) :
		curr(NULL),
		last(NULL),
		start(NULL),
		final(NULL),
		next(NULL),
		prev(NULL)
		{
			size_t n = bs;
			start    = memory::kind<memory::global>::acquire_as<uint8_t>( n );
			final    = start + bs;
			clear();
		}
		
		
		void io_block:: clear() throw()
		{
			curr = last = start;
		}
		
		size_t io_block:: length() const throw()
		{
			return last - curr;
		}
		
		size_t io_block:: unused() const throw()
		{
			return final - last;
		}
		
		size_t io_block:: offset() const throw()
		{
			return curr - start;
		}
		
		const void * io_block:: get_address() const throw() { return curr; }
		
		
		size_t io_block:: recv( io_socket &sock )
		{
			size_t nr = sock.recv( last, unused() );
			last += nr;
			return nr;
		}
		
		void io_block:: defrag() throw()
		{
			if( curr > start ) 
			{
				const size_t n = length();
				memmove(start, curr, n);
				curr = start;
				last = curr + n;
			}
		}
		
		bool io_block:: try_steal( io_block &blk ) throw()
		{
			assert( this != &blk );
			const size_t n = blk.length();
			if( unused() + offset() >= n )
			{
				defrag();
				memcpy( last, blk.curr, n );
				last += n;
				blk.clear();
				return true;
			}
			else
				return false;
		}
		
		bool io_block:: sent( io_socket &sock )
		{
			size_t ns = sock.send( curr, length() );
			curr += ns;
			if( curr >= last )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		size_t io_block:: append( const void *data, size_t size ) throw()
		{
			defrag();
			const size_t nmax = unused();
			const size_t done = size <= nmax ? size : nmax;
			memcpy( last, data, done );
			last += done;
			return done;
		}
		
	}
	
}

