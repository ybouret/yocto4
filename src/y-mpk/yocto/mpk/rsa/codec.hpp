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
		
		
		class rsa_codec : public ios::istream, public ios::ostream
		{
		public:
			static const size_t prolog = 1;
			static const size_t epilog = 1;
			virtual ~rsa_codec() throw();
			
			virtual bool query( char &C );
			virtual void store( char  C );
			
		protected:
			explicit rsa_codec( const rsa_key::pointer &);
			
			ios::bitio             in;
			ios::bitio             out;
			const rsa_key::pointer pk;
			const size_t           maxbits;
			natural                src;
			natural                tgt;
			int                    cache;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_codec);
		};
		
		class rsa_encoder :  public rsa_codec
		{
		public:
			explicit rsa_encoder( const rsa_key::pointer & );
			virtual ~rsa_encoder() throw();
			
			virtual void write(char C );
			virtual void flush();
						
		private:
			void emit();
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_encoder);
		};
		
		class rsa_decoder :  public rsa_codec
		{
		public:
            static const size_t quantum = 1+prolog+epilog+8;
			explicit rsa_decoder( const rsa_key::pointer & );
			virtual ~rsa_decoder() throw();
			
			virtual void write(char C);
			virtual void flush();
			
		private:
            ios::bitio code;
			void emit();
            void decode();
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_decoder);
		};
		
		
	}
	
}

#endif
