#ifndef YOCTO_ASSOC_MULTI_MAP_INCLUDED
#define YOCTO_ASSOC_MULTI_MAP_INCLUDED

#include "yocto/container/container.hpp"
#include "yocto/type/key.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/core/list.hpp"
#include "yocto/memory/slab.hpp"

namespace yocto
{
    namespace hidden
    {
        extern const char multi_map_name[];
    }
    
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
    typename ALLOCATOR  = memory::global::allocator >
    class multi_map
    {
    public:
        YOCTO_ASSOCIATIVE_KEY_T;
        
        class DNode
        {
        public:
            DNode *prev;
            DNode *next;
            type   data;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DNode);
        };
        
        class GNode
        {
        public:
            GNode *prev;
            GNode *next;
            DNode *dnode;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(GNode);
        };
        
        
        
        
    private:
        
    };
}

#endif
