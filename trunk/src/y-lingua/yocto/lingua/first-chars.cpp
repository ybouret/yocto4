#include "yocto/lingua/first-chars.hpp"


namespace yocto
{

    namespace lingua
    {
     
        
        first_chars:: ~first_chars() throw() {}
        
        first_chars:: first_chars() throw() :
        byte_store(),
        accept_empty(true)
        {
            
        }
        
        first_chars:: first_chars( const first_chars &fch) :
        byte_store(fch),
        accept_empty(fch.accept_empty)
        {
            
        }
        
        
    }
}
