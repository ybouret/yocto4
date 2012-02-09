#ifndef YOCTO_MPK_RSA_CODEC_INCLUDED
#define YOCTO_MPK_RSA_CODEC_INCLUDED 1

#include "yocto/mpk/rsa/keys.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
	
	namespace mpk
	{
		
		
		class rsa_codec
		{
		public:
			virtual ~rsa_codec() throw();
			
			
			
		protected:
			explicit rsa_codec( const rsa_key::pointer &);
			
			ios::bitio             in;
			ios::bitio             out;
			const rsa_key::pointer pk;
			natural                src;
			natural                tgt;
			int                    cache;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_codec);
		};
		
		class rsa_encoder : public ios::ostream, public ios::istream, public rsa_codec
		{
		public:
			explicit rsa_encoder( const rsa_key::pointer & );
			virtual ~rsa_encoder() throw();
			
			virtual void write(char C );
			virtual void flush();
			
			virtual bool query( char &C );
			virtual void store( char  C );
			
		private:
			void emit();
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_encoder);
		};
		
	}
	
}

#endif
