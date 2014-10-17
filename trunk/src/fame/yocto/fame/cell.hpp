#ifndef YOCTO_FAME_CELL_INCLUDED
#define YOCTO_FAME_CELL_INCLUDED 1

#include "yocto/fame/vertices.hpp"
#include "yocto/sequence/addr-list.hpp"


namespace yocto
{
    
    namespace fame
    {
        
        template <size_t DIM,typename T>
        class CellBase
        {
        public:
            typedef Vertex<DIM,T>   VTX;
            typedef addr_node<VTX>  VNODE;
            typedef addr_list<VTX>  VLIST;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(CellBase);
        };
        
    }
    
}

#endif
