#include "yocto/codec/lzy.hpp"

namespace yocto
{
	namespace codec
	{
		
		
		lzy:: encoder:: encoder( const writer &w ) :
		filter(w),
		queue_(),
		cache_()
		{
		}
		
		
		lzy:: encoder:: ~encoder() throw()
		{
			
		}
		
		
		void lzy:: encoder:: write( char C )
		{
			emit();
			const size_t mark = queue_.size();
			try 
			{
				queue_.push( true );  //-- has data
				queue_.push( true  );  //-- not encoded
				queue_.push_full<uint8_t>( C );
			}
			catch( ... )
			{
				while( queue_.size() > mark ) queue_.pop_back();
				throw;
			}
		}
		
		void lzy:: encoder:: flush()
		{
			try
			{
				queue_.push( false );            //-- this is the end
				queue_.fill_to_byte_with(false); //-- fill, atomic
			}
			catch(...)
			{
				queue_.pop_back(); //-- end marker
				throw;
			}
			assert( 0 == ( queue_.size() && 7 ) );
			emit();
		}
		
		void lzy:: encoder:: emit()
		{
			//-- gather bits from queue
			while( queue_.size() >= 8 )
			{
				cache_.push_back( char( queue_.peek_full<uint8_t>() ) );
				queue_.skip( 8 ); //-- OK
			}
			
			//-- try to write cache
			while( cache_.size() > 0 )
			{
				output( cache_.front() );
				cache_.pop_front();
			}
			
		}
	}
	
}