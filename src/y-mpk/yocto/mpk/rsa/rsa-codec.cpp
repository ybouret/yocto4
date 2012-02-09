#include "yocto/mpk/rsa/codec.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/exception.hpp"


#include <iostream>

namespace yocto
{
	
	namespace mpk
	{
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
		rsa_codec:: ~rsa_codec() throw()
		{
			
		}
		
		
		rsa_codec:: rsa_codec( const rsa_key::pointer &handle ) :
		in(),
		out(),
		pk( handle ),
		maxbits( pk->maxbits ),
		src(),
		tgt(),
		cache(-1)
		{
			
		}
		
		bool rsa_codec:: query( char &C )
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
		
		void rsa_codec:: store( char C )
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
		
		
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
		
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
			for( size_t i=prolog;i>0;--i)
				in.push( bits() );
			
			//--------------------------------------------------------------
			// add the byte
			//--------------------------------------------------------------
			in.push_full<uint8_t>(C);
			
			//--------------------------------------------------------------
			// add crypto noise
			//--------------------------------------------------------------
			for( size_t i=epilog;i>0;--i)
				in.push( bits() );
			
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
			while( 0 != ( in.size() % maxbits ) )
				in.push( bits() );
			
			//--------------------------------------------------------------
			// convert the input => output
			//--------------------------------------------------------------
			emit();
			
			assert( 0 == in.size() );
			
			//--------------------------------------------------------------
			// pad output
			//--------------------------------------------------------------
			while( 0 != ( out.size() & 7 ) )
			{
				out.push( bits() );
			}
			
		}
		
		
		void rsa_encoder:: emit() 
		{
			while( in.size() >= maxbits )
			{
				src = natural::query( in, maxbits );
				tgt = pk->compute( src );
				src.ldz();
				tgt.store( out, maxbits );
				tgt.ldz();
			}
		}
		
				
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
		
		rsa_decoder:: rsa_decoder( const rsa_key::pointer &handle ) :
		rsa_codec( handle )
		{
		}
		
		rsa_decoder:: ~rsa_decoder() throw()
		{
		}
		
		void rsa_decoder:: write( char C )
		{
			
		}
		
		void rsa_decoder:: flush()
		{
			
		}
		
	}
	
}