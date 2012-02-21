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
        key( handle ),
        Q()
        {
			
		}
        
        
        bool rsa_codec:: query( char &C )
        {
            if( Q.size() <= 0 )
                return false;
            else
            {
                C = Q.front();
                Q.pop_front();
                return true;
            }
        }
        
        void rsa_codec:: store( char C )
        {
            Q.push_front(C);
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
                //std::cerr << "rsa.encode : " << P << "/" << P.bits() << " => " << C << "/" << C.bits() << std::endl;
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
        
        void rsa_decoder:: decode()
        {
            while( coded.size() >= key->obits )
            {
                const natural C = natural::query( coded, key->obits );
                const natural P = key->compute( C );
                P.store( plain, key->ibits );
                //std::cerr << "rsa.decode : " << C << "/" << C.bits() << " => " << P << "/" << P.bits() << std::endl;
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
                        //std::cerr << "[" << char( Q.back() ) << "]" << std::endl;
                        
                    }
                    else
                    {
                        //-- not enough bits so far
                        //std::cerr << "<.>" << std::endl;
                        return;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // it was flushed
                    //----------------------------------------------------------
                    //std::cerr << "EOF/+#plain=" << plain.size() << " /+#coded=" << coded.size() << std::endl;
                    // std::cerr << "EOF" << std::endl;
                    // remove meaning less plain bits
                    plain.free();
                    
                    //  remove padding bits
                    coded.free();
                    //while( 0 != ( coded.size() & 7 ) ) coded.skip();
                    
                    return;
                }
            }
        }
        
    }
    
}
