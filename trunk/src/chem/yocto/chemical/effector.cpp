#include "yocto/chemical/effector.hpp"

namespace yocto
{
    namespace chemical
    {
        effector:: ~effector() throw()
        {
        }
     
        effector::effector( const string &id ) :
        name(id),
        Vmax(1)
        {
            
        }
        
        
        //======================================================================
        //
        // Effectors
        //
        //======================================================================

        effectors:: effectors() throw() : effector::database()
        {
            
        }
        
        effectors:: ~effectors() throw()
        {
            
        }
    
        
    }
}
