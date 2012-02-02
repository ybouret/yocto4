#include "yocto/net/io-queue.hpp"
#include "yocto/code/round.hpp"

#include <cstring>
#include <iostream>

namespace yocto
{
	namespace network
	{
		
		io_queue:: ~io_queue() throw()
		{
			while( send_blocks.size ) delete send_blocks.pop_back();
			while( recv_blocks.size ) delete recv_blocks.pop_back();
			while( pool_blocks.size ) delete pool_blocks.query();
		}
		
		static inline size_t __block_size( size_t bs )
		{
			if( bs < 16 ) bs = 16;
			return YOCTO_ROUND16(bs);
		}
		
		io_queue:: io_queue( size_t bs ) :
		block_size( __block_size(bs) ),
		send_blocks(),
		recv_blocks(),
		pool_blocks()
		{
			
		}
		
		
		io_block * io_queue:: fetch( )
		{
			static int cnt = 0;
			if( pool_blocks.size > 0 )
			{
				io_block *blk = pool_blocks.query();
				blk->clear();
				return blk;
			}
			else
			{
				std::cerr << "<" << ++cnt << ">" << std::endl;
				return new io_block( block_size );
			}
			
		}
		
		
		bool io_queue:: recv( io_socket &sock )
		{
			io_block *blk = fetch();
			try
			{
				if( blk->recv(sock) )
				{
					//----------------------------------------------------------
					// append data to recv block
					//----------------------------------------------------------
					io_block *tail = recv_blocks.tail;
					
					if( tail && tail->try_steal( *blk ) )
					{
						//-- compacted
						pool_blocks.store(blk);
						std::cerr << "recv:compacted" << std::endl;
					}
					else
					{
						//-- keep the new block
						recv_blocks.push_back( blk );
						std::cerr << "recv:+1" << std::endl;
					}
					
					return true;
				}
				else
				{
					std::cerr << "recv:0" << std::endl;
					pool_blocks.store(blk);
					return false;
				}
				
			}
			catch(...)
			{
				pool_blocks.store(blk);
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
					pool_blocks.store( send_blocks.pop_front() );
					std::cerr << "sent:+1" << std::endl;
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
				io_block *blk = recv_blocks.head;
				assert( blk->length() > 0 );
				C = *(blk->curr++);
				if( blk->length() <= 0 )
				{
					std::cerr << "query:-1" << std::endl;
					pool_blocks.store( recv_blocks.pop_front() );
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		
		void io_queue:: store( char C )
		{
			io_block *blk = NULL;
			if( recv_blocks.size <=0 || recv_blocks.head->offset() <= 0 )
			{
				std::cerr << "store:+1" << std::endl;
				blk       = fetch();
				blk->curr = blk->last = (uint8_t*)(blk->final);
			}
			else
			{
				assert( recv_blocks.head != NULL );
				blk = recv_blocks.head;
				assert( blk->offset() > 0 );
			}
			*( --(blk->curr) ) = C;
			
		}
		
		void io_queue:: reset() throw()
		{
			while( send_blocks.size ) pool_blocks.store( send_blocks.pop_back() );
			while( recv_blocks.size ) pool_blocks.store( recv_blocks.pop_back() );
		}
		
		void io_queue:: get( void *data, size_t size, size_t &done )
		{
			uint8_t *p = (uint8_t *)data;
			done = 0;
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
					std::cerr << "get:-1" << std::endl;
					pool_blocks.store( recv_blocks.pop_front() );
					if( (done += blen) >= size )
						return;
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
					return;
				}
			}
		}
		
		
		void io_queue:: write( char C )
		{
			if( send_blocks.size <= 0 || send_blocks.tail->unused() == 0 )
			{
				std::cerr << "write: +1" << std::endl;
				send_blocks.push_back( fetch() );
			}
			io_block *blk = send_blocks.tail; 	assert( blk->unused() > 0 );
			*(blk->last++) = C;
		}
		
		
		void io_queue:: flush()
		{
			
		}
		
		void io_queue:: put( const void *data, size_t size, size_t &done)
		{
			//std::cerr << "<put " << size << ">" << std::endl;
			done = 0 ;
			if( size > 0 )
			{
				const uint8_t *p = (uint8_t *) data;
				if( send_blocks.size <= 0 )
				{
					send_blocks.push_back( fetch() );
					std::cerr << "put:+1" << std::endl;
				}
				
				io_block *blk = send_blocks.tail;
				for(;;)
				{
					assert( done < size );
					const size_t ns = blk->append( &p[done], size-done);
					done += ns;
					if( done >= size )
						return;
					send_blocks.push_back( fetch() );
					std::cerr << "put:+1" << std::endl;
					blk = send_blocks.tail;
				}
			}
			
		}
		
		
		size_t io_queue:: cache_size() const throw() { return pool_blocks.size * block_size; }
		
		
	}
	
	
}
