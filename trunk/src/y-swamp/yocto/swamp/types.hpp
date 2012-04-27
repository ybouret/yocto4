// \file
/**
 Simplified Workspaces and Meshes Processing
 */

#ifndef YOCTO_SWAMP_TYPES_INCLUDED
#define YOCTO_SWAMP_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        
        //! signed integer for indices
        typedef ptrdiff_t              unit_t;
       
        //! base class for list of offsets
        typedef sorted_vector<size_t>  sorted_offsets;
       
        class offsets : public sorted_offsets
        {
        public:
            explicit offsets() throw();
            explicit offsets(size_t num);
            virtual ~offsets() throw();
            offsets( const offsets & );
            
            void store( size_t indx );
            
        private:
            YOCTO_DISABLE_ASSIGN(offsets);
        };
        
        
    }
    
}

#endif
