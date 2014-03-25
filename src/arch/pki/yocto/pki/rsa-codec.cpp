#include "yocto/pki/rsa-codec.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/exception.hpp"


#include <iostream>

namespace yocto
{
	
	namespace pki
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
        q_codec(),
        key( handle )
        {
			
		}
        
        
        
        
		////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
        rsa_encoder:: rsa_encoder(const rsa_key::pointer &handle) :
        rsa_codec( handle ),
        plain(),
        coded()
        {}
        
        void rsa_encoder:: reset() throw()
        {
            Q.free();
            plain.free();
            coded.free();
        }
        
        rsa_encoder:: ~rsa_encoder() throw()
        {}
        
        
        void rsa_encoder:: write( char C ) 
        {
            static Random::Bits & noise = Random::CryptoBits();
            
            //------------------------------------------------------------------
            // this is a byte
            //------------------------------------------------------------------
            plain.push( true );
            
            //------------------------------------------------------------------
            // make prolog noise
            //------------------------------------------------------------------
            for( size_t i=0; i < prolog; ++i ) plain.push( noise() );
            
            //------------------------------------------------------------------
            // push data
            //------------------------------------------------------------------
            plain.push_full<uint8_t>( C );
            
            
            encode();
        }
        
        
        void rsa_encoder:: flush()
        {
            static Random::Bits & noise = Random::CryptoBits();
            
            //------------------------------------------------------------------
            // this is the end of one block
            //------------------------------------------------------------------
            plain.push( false );
            
            //------------------------------------------------------------------
            // pad to key->maxbits
            //------------------------------------------------------------------
            while( 0 != ( plain.size() % key->ibits ) )
                plain.push( noise() );
            
            //------------------------------------------------------------------
            // encode
            //------------------------------------------------------------------
            encode();
            assert( 0 == plain.size() );
            
            //------------------------------------------------------------------
            // pad coded bits to byte
            //------------------------------------------------------------------
            while( 0 != ( coded.size() & 7 ) ) coded.push( noise() );
            pack();
            assert(0==coded.size());
        }
        
        void rsa_encoder:: encode()
        {
            while( plain.size() >= key->ibits )
            {
                const natural P = natural::query( plain, key->ibits );
                assert( P < key->modulus );
                const natural C = key->compute(P);
                C.store( coded, key->obits );
                pack();
            }
        }
        
        void rsa_encoder:: pack()
        {
            while( coded.size() >= 8 )
            {
                Q.push_back( coded.pop_full<uint8_t>() );
            }
            
        }
        
        ////////////////////////////////////////////////////////////////////////
		//
		//
		//
		////////////////////////////////////////////////////////////////////////
        rsa_decoder:: rsa_decoder(const rsa_key::pointer &handle) :
        rsa_codec( handle ),
        coded(),
        plain()
        {}
        
        rsa_decoder:: ~rsa_decoder() throw()
        {}
        
        
        void rsa_decoder:: write( char C ) 
        {
            const uint8_t B(C);
            coded.push_full<uint8_t>( B );
            decode();
        }
        
        void rsa_decoder:: flush()
        {
            
        }
        
        void rsa_decoder:: reset() throw()
        {
            Q.free();
            plain.free();
            coded.free();
        }

        
        void rsa_decoder:: decode()
        {
            while( coded.size() >= key->obits )
            {
                const natural C = natural::query( coded, key->obits );
                const natural P = key->compute( C );
                P.store( plain, key->ibits );
                unpack();
            }
        }
        
        
        void rsa_decoder:: unpack()
        {
            while( plain.size() > 0 )
            {
                if( plain.peek() )
                {
                    //----------------------------------------------------------
                    // it was a byte
                    //----------------------------------------------------------
                    if( plain.size() >= dblock )
                    {
                        //-- drop prolog+flag
                        plain.skip( prolog+1 );
                        
                        //-- collect byte => Q
                        Q.push_back( plain.pop_full<uint8_t>() );
                        
                    }
                    else
                    {
                        //-- not enough bits so far
                        return;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // it was flushed
                    //----------------------------------------------------------
                    // remove meaning less plain bits
                    plain.free();
                    
                    //  remove padding bits
                    coded.free();
                    
                    return;
                }
            }
        }
        
    }
    
}
