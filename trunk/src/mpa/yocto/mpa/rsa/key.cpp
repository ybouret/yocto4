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
            
            const as_primes_t as_primes = {};
            
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
                if(publicExponent>=modulus)
                    throw exception("invalid public key");
            }
            
            PublicKey:: PublicKey(const natural &p,
                                  const natural &q,
                                  const as_primes_t & ) :
            Key( p*q ),
            publicExponent()
            {
                
            }
            
            
            
            
        }
        
        
    }
    
}
