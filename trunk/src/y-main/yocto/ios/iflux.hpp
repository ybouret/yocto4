#ifndef YOCTO_IFLUX_INCLUDED
#define YOCTO_IFLUX_INCLUDED 1

#include "yocto/ios/istream.hpp"
#include "yocto/memory/buffer.hpp"

namespace yocto
{
	
	namespace ios
	{
        
        //! istream decorator, from an ichannel
		class iflux : public istream
		{
		public:
			virtual ~iflux() throw();
			explicit iflux( ios::ichannel &ch, memory::rw_buffer &buff );
			
			virtual bool query( char &C );
			virtual void store( char C );
			
		private:
			ios::ichannel  &in_;
			char           *init_;
			char           *curr_;
			const size_t    size_;
			const char     *last_;
			bool            fetch();
			YOCTO_DISABLE_COPY_AND_ASSIGN(iflux);
		};
	}
}
#endif
