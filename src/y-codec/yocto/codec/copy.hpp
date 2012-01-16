#ifndef YOCTO_CODEC_COPY_INCLUDED
#define YOCTO_CODEC_COPY_INCLUDED 1

#include "yocto/codec/codec.hpp"
#include "yocto/sequence/list.hpp"


namespace yocto
{
	
	namespace codec
	{
		
		struct copy
		{
			class encoder : public filter
			{
			public:
				explicit encoder( const writer &w, size_t cache_max = 0);
				virtual ~encoder() throw();
			
				virtual void write( char C );
				virtual void flush();
				
			private:
				list<char>   list_;
				const size_t keep_;
				void         emit();
				YOCTO_DISABLE_COPY_AND_ASSIGN(encoder);
			};
			
			typedef encoder decoder;
			
		};
		
	}
	
}

#endif
