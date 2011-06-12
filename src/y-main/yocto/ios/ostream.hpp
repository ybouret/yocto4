#ifndef YOCTO_IOS_OSTREAM_INCLUDED
#define YOCTO_IOS_OSTREAM_INCLUDED 1

#include "yocto/ios/stream.hpp"
#include "yocto/code/endian.hpp"

namespace yocto
{
	namespace ios 
	{
		class ostream : public stream
		{
		public:
			virtual ~ostream() throw();
			
			virtual void write( char C ) = 0;
			virtual void flush()         = 0;
			virtual size_t put( const void *buffer, size_t buflen );
			
			void save( const void *buffer, size_t buflen );
			template <typename T>
			inline void emit( T x ) { x = swap_be<T>(x); save(&x,sizeof(T)); } 
			
		protected:
			explicit ostream() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ostream);
		};
		
	}
	
}
#endif
