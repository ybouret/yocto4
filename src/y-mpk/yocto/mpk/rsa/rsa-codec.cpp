#include "yocto/mpk/rsa/codec.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/exception.hpp"


#include <iostream>

namespace yocto
{
	
	namespace mpk
	{
		
		rsa_codec:: ~rsa_codec() throw()
		{
			
		}
		
		
		rsa_codec:: rsa_codec( const rsa_key::pointer &handle ) :
		in(),
		out(),
		pk( handle ),
		src(),
		tgt(),
		cache(-1)
		{
			
		}
		
		
		rsa_encoder:: rsa_encoder( const rsa_key::pointer &handle ) :
		rsa_codec( handle )
		{
		}
		
		rsa_encoder:: ~rsa_encoder() throw()
		{
		}
		
		void rsa_encoder:: write( char C )
		{
			static Random::Bits &bits = Random::CryptoBits();
			
			//--------------------------------------------------------------
			// we have data
			//--------------------------------------------------------------
			in.push( true );
			
			//--------------------------------------------------------------
			// add crypto noise
			//--------------------------------------------------------------
			in.push( bits() );
			
			//--------------------------------------------------------------
			// add the byte
			//--------------------------------------------------------------
			in.push_full<uint8_t>(C);
			
			//--------------------------------------------------------------
			// convert the input => output
			//--------------------------------------------------------------
			emit();
			
		}
		
		void rsa_encoder:: flush() 
		{
			static Random::Bits &bits = Random::CryptoBits();
			//--------------------------------------------------------------
			// we don't have data
			//--------------------------------------------------------------
			in.push( false );
			
			//--------------------------------------------------------------
			// round up
			//--------------------------------------------------------------
			const size_t nbits = pk->maxbits;
			while( 0 != ( in.size() % nbits ) )
				in.push( bits() );
			
			//--------------------------------------------------------------
			// convert the input => output
			//--------------------------------------------------------------
			emit();
		}
		
		
		void rsa_encoder:: emit() 
		{
			const size_t nbits = pk->maxbits;
			while( in.size() >= nbits )
			{
				src = natural::query( in, nbits );
				std::cerr << "src: " << src << std::endl;
				tgt = pk->compute( src );
				std::cerr << "tgt: " << tgt << std::endl;
				src.ldz();
				tgt.store( out, nbits );
				tgt.ldz();
			}
		}
		
		bool rsa_encoder:: query( char &C )
		{
			if( cache >= 0 )
			{
				C = char(cache);
				cache = -1;
				return true;
			}
			else
			{
				assert( -1 == cache );
				if( out.size() >= 8 )
				{
					C = char( out.pop_full<uint8_t>() );
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		
		void rsa_encoder:: store( char C )
		{
			if( cache < 0 )
			{
				cache = uint8_t(C);
			}
			else 
			{
				throw exception("rsa_encoder.store(already cached)");
			}
			
		}
		
		
	}
	
}