#ifndef YOCTO_MPK_RSA_CODEC_INCLUDED
#define YOCTO_MPK_RSA_CODEC_INCLUDED 1

#include "yocto/pki/rsa-keys.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/ios/codec.hpp"

namespace yocto
{
	
	namespace pki
	{
		
		
		class rsa_codec : public ios::codec
		{
		public:
			static const size_t prolog = 1; //!< #random bits before byte
            static const size_t dblock = 1 + prolog + 8; //!< #bits to encode one byte
			virtual ~rsa_codec() throw();
			
            virtual bool query( char &C );
            virtual void store( char C );
			
		protected:
			explicit rsa_codec( const rsa_key::pointer &);
			const rsa_key::pointer key;
            list<char>             Q;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_codec);
		};
		
        
        class rsa_encoder : public rsa_codec
        {
        public:
            explicit rsa_encoder( const rsa_key::pointer &);
            virtual ~rsa_encoder() throw();
            
            virtual void write( char C );
            virtual void flush();
            
            virtual void reset() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_encoder);
            void encode(); //!< plain->coded
            void pack();   //!< coded->Q
            
            ios::bitio plain;
            ios::bitio coded;
        };
		
		
        class rsa_decoder : public rsa_codec
        {
        public:
            explicit rsa_decoder(  const rsa_key::pointer &);
            virtual ~rsa_decoder() throw();
            
            virtual void write( char C );
            virtual void flush();
            virtual void reset() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rsa_decoder);
            void decode(); //!< coded -> plain
            void unpack(); //!< plain -> Q
            
            ios::bitio coded;
            ios::bitio plain;
            
        };
		
	}
	
}

#endif
