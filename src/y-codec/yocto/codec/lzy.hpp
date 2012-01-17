#ifndef YOCTO_CODEC_LZY_INCLUDED
#define YOCTO_CODEC_LZY_INCLUDED 1

#include "yocto/codec/codec.hpp"
#include "yocto/ios/bitio.hpp"

namespace yocto
{
	
	namespace codec
	{
		
		struct lzy
		{
			
			class encoder : public filter
			{
			public:
				static const char ID[];
				explicit encoder( const writer &w );
				virtual ~encoder() throw();
				
				virtual void        write( char C );
				virtual void        flush();
				virtual const char *name() const throw();
				
			private:
				ios::bitio queue_; //!< temporary queue
				list<char> cache_; //!< 8 bits from queue
				bool       flush_; //!< are we flushing ?
				void emit();
				YOCTO_DISABLE_COPY_AND_ASSIGN(encoder);
				
			};
			
			
		};
		
	}
	
}

#endif
