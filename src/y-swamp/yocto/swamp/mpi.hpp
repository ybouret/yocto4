#ifndef YOCTO_SWAMP_MPI_INCLUDED
#define YOCTO_SWAMP_MPI_INCLUDED 1


#include "yocto/swamp/dataspace.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        
        
        struct _mpi
        {
            template <typename LAYOUT>
            inline void start_exchange( dataspace<LAYOUT> &D, mpi::Requests &requests )
            {
                //==============================================================
                // exchange localghosts
                //==============================================================
                for( size_t i= D.local_ghosts_count(); i>0; --i )
                {
                    
                }
            }
            
        };
        
        
        
    }
    
}

#endif
