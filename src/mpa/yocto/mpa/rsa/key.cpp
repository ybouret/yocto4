#include "yocto/mpa/rsa/key.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace mpa
    {
        
        namespace RSA
        {
            
            //__________________________________________________________________
            //
            // Key
            //__________________________________________________________________
            
            Key:: ~Key() throw()
            {
                modulus.__clr();
            }
            
            Key:: Key( const natural &Modulus ) :
            modulus( Modulus )
            {
            }
            
            Key:: Key( const Key & other ) :
            modulus( other.modulus )
            {
            }
            
            
            //__________________________________________________________________
            //
            // PublicKey
            //__________________________________________________________________
            PublicKey:: ~PublicKey() throw()
            {
                publicExponent.__clr();
            }
            
            PublicKey:: PublicKey(const natural &Modulus,
                                  const natural &PublicExponent) :
            Key(Modulus),
            publicExponent(PublicExponent)
            {
                
            }
            
            PublicKey:: PublicKey( const PublicKey &other ) :
            Key( other ),
            publicExponent( other.publicExponent )
            {
            }
            
            
            PublicKey PublicKey:: GenerateFrom( const natural &prime1, const natural &prime2, const natural &exponent )
            {
                const natural n = prime1 * prime2;
                const natural phi = (prime1-1) * (prime2-1);
                natural e = exponent;
                if(e<2) e = 2;
                while( ! mpn::are_coprime(e,phi) )
                    ++e;
                return PublicKey(n,e);
            }

            
            
        }
        
        
    }
    
}
