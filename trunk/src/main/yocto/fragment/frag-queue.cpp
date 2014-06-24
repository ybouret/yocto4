#include "yocto/fragment/queue.hpp"
#include "yocto/object.hpp"
#include "yocto/exceptions.hpp"


namespace yocto
{
	
	namespace fragment
	{
		
		
		queue::queue(size_t user_block_size) throw() :
		layout( user_block_size, __log2 ),
		bytes_(0),
		list_(),
		pool_()
		{
			
		}
		
		queue:: ~queue() throw() { release(); }
		
		void queue:: free() throw()
		{
			while( list_.size )
			{
				pool_.store( list_.pop_back() );
			}
			bytes_ = 0;
		}
		
		void queue:: release() throw()
		{
			while( list_.size )
			{
				block *blk = list_.pop_back();
				destruct(blk);
				object::release1<block>(blk);
			}
			
			while( pool_.size )
			{
				block *blk = pool_.query();
				destruct(blk);
				object::release1<block>(blk);
			}
			
			bytes_ = 0;
		}
		
		static inline
		block *new_block( size_t block_size )
		{
			block *blk = object::acquire1<block>();
			try {
				new (blk) block(block_size);
			}
			catch(...)
			{
				object::release1<block>(blk);
				throw;
			}
			return blk;
		}
		
		block *queue:: fetch_block()
		{
			if( pool_.size )
			{
				block *blk = pool_.query();
				blk->clear();
				return blk;
			}
			else 
			{
				return new_block( block_size );
			}
		}
		
		block * queue:: get_block_for( size_t n )
		{
			assert( n <= block_size );
			block *blk = list_.tail;
			if( !blk || blk->unused() < n )
			{
				blk = fetch_block();
				list_.push_back( blk );
			}
			assert(blk);
			return blk;
		}
		
		void  queue:: dispatch( void *frag_data, size_t frag_size, void *args)
		{
			queue       *q   = (queue *)args;
			block       *blk = q->get_block_for(frag_size);
			const size_t nw  = blk->write(frag_data, frag_size);
			q->bytes_ += nw;
			
			if( nw < frag_size ) throw exception("fragment::queue::dispatch FAILURE");
			
		}
		
		void queue:: put( const void *data, size_t size, size_t &done ) 
		{
			assert(!(data==NULL&&size>0) );
			const size_t num_bytes  = bytes_;
			
			try {
				process((void*)data, size, dispatch, this);
				done = size;
			}
			catch (...) {
				done = bytes_ - num_bytes;
				throw;
			}
		}
		
		void queue:: write( char C )
		{
			if( get_block_for(1)->write( &C, 1) < 1 )
				throw exception("fragment::queue::write FAILURE");
			++bytes_;
		}
		
		void queue:: flush()
		{
			
		}
		
		size_t queue:: bytes() const throw()
		{
			return bytes_;
		}
		
		size_t queue:: get( void *data, size_t size )
		{
			assert(!(data==NULL&&size>0) );
			uint8_t *p = (uint8_t *)data;
			size_t   n = size;
			
			while( n > 0 )
			{
				block *blk = list_.head;
				if( !blk ) 
					break;				
				const size_t nr = blk->read(p,n);
				p += nr;
				n -= nr;
				if( 0 == blk->length() ) 
				{
					pool_.store( list_.pop_front() );
				}
			}
			
			const size_t done    = size - n;
			assert( done <= bytes_ );
			bytes_ -= done;
            return done;
		}
		
		bool queue:: query( char &C )
		{
			return get( &C, 1) !=0 ;
		}
		
		void queue:: store( char C )
		{
			if( list_.head || ! list_.head->back(C) )
			{
				list_.push_front( fetch_block() );
				assert( list_.head );
				if( !list_.head->back(C) )
					throw exception("queue::store unexpected failure!");
			}
		}
		
		
		bool queue:: recv( ios::ichannel &input )
		{
			block *blk = fetch_block();
			try
			{
				if( blk->recv(input) )
				{
					list_.push_back(blk);
					return true;
				}
				else
				{
					pool_.store(blk);
					return false;
				}
			}
			catch(...)
			{
				pool_.store(blk);
				throw;
			}
		}
		
	}
	
}
