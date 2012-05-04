#include "yocto/swamp/field-layout.hpp"

namespace yocto 
{
    namespace swamp 
    {
        field_info:: field_info(const char          *id, 
                                const std::type_info &which, 
                                const bool            asynchronous ) :
        name(id),
        spec(which),
        async(asynchronous)
        {
        }
        
        field_info:: ~field_info() throw()
        {}
        
        
        field_info:: field_info( const field_info &other  ) : 
        name(  other.name  ),
        spec(  other.spec  ),
        async( other.async )
        {
        }
        
        field_layout:: field_layout() throw() :
        vector<field_info>()
        {
            
        }
        
        field_layout:: ~field_layout() throw()
        {
        }
        
        field_layout:: field_layout( const field_layout &other) :
        vector<field_info>(other)
        {
        }
        
    }
}
