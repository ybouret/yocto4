#ifndef YOCTO_MPK_RSA_CODEC_INCLUDED
#define YOCTO_MPK_RSA_CODEC_INCLUDED 1

#include "yocto/mpk/rsa/keys.hpp"
#include "yocto/ios/bitio.hpp"

namespace yocto
{
	
	namespace mpk
	{
		
		class rsa_codec
		{
		public:
			virtual ~rsa_codec() throw();
			
		protected:
			explicit      rsa_codec();
			ios::bitio    in;
			list<uint8_t> out;
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_codec);
		};
		
		class rsa_encoder : public rsa_codec
		{
		public:
			explicit rsa_encoder();
			virtual ~rsa_encoder() throw();
			
			virtual void write(char C );
			virtual void flus() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_encoder);
		};
		
	}
	
}

#endif
