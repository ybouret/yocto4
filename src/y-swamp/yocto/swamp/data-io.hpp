#ifndef YOCTO_SWAMP_DATA_IO_INCLUDED
#define YOCTO_SWAMP_DATA_IO_INCLUDED 1

#include "yocto/swamp/dataspace.hpp"

namespace yocto 
{
    namespace swamp 
    {
        struct dataIO
        {
            
            
            //! collect virtual arrays with name in var
            template <typename LAYOUT> 
            static inline
            void make_loader(sequence<varray*>  &ld, 
                             dataspace<LAYOUT>   &D, 
                             const array<string> &var )
            {
                const size_t num_var = var.size();
                ld.free();
                ld.reserve( num_var );
                for( size_t i=1; i <= num_var; ++i )
                {
                    ld.push_back( & D[ var[i] ] );
                }
            }
            
            template <typename T,typename ARRAY>
            static inline
            void process( const array<varray*> &ld, dataspace<typename ARRAY::layout_type> &D )
            {
                const size_t num_var = ld.size();
                for( size_t i=num_var;i>0;--i)
                {
                    ARRAY &A = ld[i]->as<ARRAY>();
                }
                
            }
            
            
        };
        
    }
}

#endif
