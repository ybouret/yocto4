#ifndef YOCTO_SWAMP_WORKSPACE_INCLUDED
#define YOCTO_SWAMP_WORKSPACE_INCLUDED 1

#include "yocto/swamp/dataspace.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        //! a workspace based on data+mesh
        template <
        typename LAYOUT,
        typename U,
        template <typename,typename> class QUADMESH
        >
        class workspace : public dataspace<LAYOUT>
        {
        public:
            typedef QUADMESH<U,LAYOUT> mesh_type;
            typedef typename dataspace<LAYOUT>::coord coord;
            mesh_type mesh;
            
            explicit workspace( const LAYOUT              &L,
                               const ghosts_setup<coord>  &G,
                               const fields_setup<LAYOUT> &F) :
            dataspace<LAYOUT>(L,G,F),
            mesh( *this, this->outline )
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
        };
    }
}


#endif
