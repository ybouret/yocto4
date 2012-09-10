#ifndef YOCTO_SPADE_WORKSPACE_INCLUDED
#define YOCTO_SPADE_WORKSPACE_INCLUDED 1

#include "yocto/spade/dataspace.hpp"

namespace yocto
{
    namespace spade
    {
        
        template <
        typename LAYOUT,
        template <class,class> class MESH,
        typename U>
        class workspace : public dataspace<LAYOUT>
        {
        public:
            MESH<LAYOUT,U> mesh;
            
            explicit workspace(const LAYOUT               &L,
                               const fields_setup<LAYOUT> &F,
                               const ghosts_setup         &G) :
            dataspace<LAYOUT>(L,F,G),
            mesh( this->outline, *this )
            {
                
            }
            
            virtual ~workspace() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
        };
    }
}

#endif
