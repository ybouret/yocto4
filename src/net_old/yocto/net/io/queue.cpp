#include "yocto/net/io/queue.hpp"
#include "yocto/code/round.hpp"

#include <cstring>

namespace yocto
{
	namespace network
	{
		
		io_queue:: ~io_queue() throw()
		{
			reset();
		}
		
		
		io_queue:: io_queue( io_cache &db ) :
		send_blocks(),
		recv_blocks(),
		recv_length(0),
		cache( db )
		{
			
		}
		
		bool io_queue:: would_send() const throw() 
		{
			return send_blocks.size > 0;
		}
		
		void io_queue:: clear_recv() throw()
		{
			while( recv_blocks.size ) cache.collect( recv_blocks.pop_back() );
			(size_t&)recv_length = 0;
		}
		
		bool io_queue:: recv( io_socket &sock )
		{
			io_block *blk = cache.provide();
			try
			{
				const size_t nr = blk->recv(sock);
				if(nr>0)
				{
					//----------------------------------------------------------
					// append data to recv block
					//----------------------------------------------------------
					io_block *tail = recv_blocks.tail;
					
					if( tail && tail->try_steal( *blk ) )
					{
						//-- compacted
						cache.collect(blk);
					}
					else
					{
						//-- keep the new block
						recv_blocks.push_back( blk );
					}
					
					(size_t&)recv_length += nr;
					return true;
				}
				else
				{
					cache.collect(blk);
					return false;
				}
				
			}
			catch(...)
			{
				cache.collect(blk);
				throw;
			}
		}
		
		bool io_queue:: sent( io_socket &sock )
		{
			if( send_blocks.size > 0 )
			{
				//--------------------------------------------------------------
				// try to send the head block
				//--------------------------------------------------------------
				io_block *blk = send_blocks.head;
				if( blk->sent(sock) )
				{
					//-- the head block is now empty
					assert( 0 == blk->length() );
					cache.collect( send_blocks.pop_front() );
					return send_blocks.size <= 0;
				}
				else
				{
					//-- couldn't send the head block in one time.
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		
		
		
		
		bool io_queue:: query( char &C )
		{
			if( recv_blocks.size > 0 )
			{
				assert(recv_length>0);
				io_block *blk = recv_blocks.head;
				assert( blk->length() > 0 );
				C = *(blk->curr++);
				if( blk->length() <= 0 )
				{
					cache.collect( recv_blocks.pop_front() );
				}
				--( (size_t&)recv_length );
				return true;
			}
			else
			{
				assert(0==recv_length);
				return false;
			}
		}
		
		void io_queue:: store( char C )
		{
			io_block *blk = NULL;
			if( recv_blocks.size <=0 || recv_blocks.head->offset() <= 0 )
			{
				blk       = cache.provide();
				blk->curr = blk->last = (uint8_t*)(blk->final);
			}
			else
			{
				assert( recv_blocks.head != NULL );
				blk = recv_blocks.head;
				assert( blk->offset() > 0 );
			}
			*( --(blk->curr) ) = C;
			++( (size_t &)recv_length );
			
		}
		
		void io_queue:: reset() throw()
		{
			while( send_blocks.size ) cache.collect( send_blocks.pop_back() );
			clear_recv();
		}
		
		void io_queue:: get( void *data, size_t size, size_t &done )
		{
			uint8_t *p = (uint8_t *)data;
			done = 0;
			try
			{
				while( recv_blocks.size > 0 )
				{
					io_block *blk = recv_blocks.head;  //-- use the fist block
					assert( blk->length() > 0 );
					const size_t todo = size - done;   //-- we need those bytes
					const size_t blen = blk->length(); //-- to take from the block
					if( blen <= todo )
					{
						//----------------------------------------------------------
						// take the whole block and remove it
						//----------------------------------------------------------
						memcpy( &p[done], blk->curr, blen );
						cache.collect( recv_blocks.pop_front() );
						if( (done += blen) >= size )
						{
							//-- update length and return
							(size_t&)recv_length += done;
							return;
						}
					}
					else
					{
						//----------------------------------------------------------
						// take todo bytes and update block
						//----------------------------------------------------------
						assert( blen > todo );
						memcpy( &p[done], blk->curr, todo );
						blk->curr += todo;
						assert( blk->length() > 0 );
						done += todo;
						
						//-- update length and return
						(size_t&)recv_length += done;
						return;
					}
				}
			}
			catch(...)
			{
				(size_t&)recv_length += done;
				throw;
			}
		}
		
		
		void io_queue:: write( char C )
		{
			if( send_blocks.size <= 0 || send_blocks.tail->unused() == 0 )
			{
				send_blocks.push_back( cache.provide() );
			}
			io_block *blk = send_blocks.tail; 	assert( blk->unused() > 0 );
			*(blk->last++) = C;
		}
		
		
		void io_queue:: flush()
		{
			
		}
		
		void io_queue:: put( const void *data, size_t size, size_t &done)
		{
			done = 0 ;
			if( size > 0 )
			{
				const uint8_t *p = (uint8_t *) data;
				if( send_blocks.size <= 0 )
				{
					send_blocks.push_back( cache.provide() );
				}
				
				io_block *blk = send_blocks.tail;
				for(;;)
				{
					assert( done < size );
					const size_t ns = blk->append( &p[done], size-done);
					done += ns;
					if( done >= size )
						return;
					send_blocks.push_back( cache.provide() );
					blk = send_blocks.tail;
				}
			}
			
		}
		
		
	}
	
	
}
