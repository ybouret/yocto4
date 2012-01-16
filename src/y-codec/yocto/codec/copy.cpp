#include "yocto/codec/copy.hpp"

namespace yocto
{
	namespace codec
	{
		
		copy::encoder:: encoder( const writer &w, const size_t cache_max ) :
		filter(w),
		list_( cache_max, as_capacity ),
		keep_( cache_max )
		{
			
		}
		
		
		copy:: encoder:: ~encoder() throw()
		{
		}
		
		void copy::encoder:: write( char C )
		{
			emit();
			list_.push_back( C );
		}
		
		void copy::encoder:: flush()
		{
			emit();
		}
		
		void copy::encoder:: emit()
		{
			while( list_.size() > 0 )
			{
				output( list_.front() );
				list_.pop_front();
			}
			list_.max_available( keep_ );
		}
		
		const char   copy:: encoder:: ID[] = "copy-encoder";
		const char * copy:: encoder:: name() const throw()
		{
			return ID;
		}
		
		
		const char   copy:: decoder:: ID[] = "copy-decoder";

		copy::decoder:: ~decoder() throw()
		{
		}
		
		const char * copy:: decoder:: name() const throw()
		{
			return ID;
		}
		
		copy:: decoder:: decoder( const writer &w, const size_t cache_max ) :
		encoder( w, cache_max )
		{
			
		}
		
		
	}
	
}