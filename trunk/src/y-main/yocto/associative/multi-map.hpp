#ifndef YOCTO_ASSOC_MULTI_MAP_INCLUDED
#define YOCTO_ASSOC_MULTI_MAP_INCLUDED

#include "yocto/associative/set.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/intr-ptr.hpp"
#include "yocto/counted.hpp"

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
        
        typedef core::list_of<DNode> DList;
        typedef core::pool_of<DNode> DPool;
        
        class GNode
        {
        public:
            GNode *prev;
            GNode *next;
            DNode *dnode;
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(GNode);
        };
        typedef core::pool_of<GNode> GPool;
        
        class Group :
        public object,
        public counted,
        public core::list_of<GNode>
        {
        public:
            typedef intr_ptr<KEY,Group> pointer;
            const_key            key;
            GPool                pool;
            
            explicit Group( param_key k ) :
            key(k)
            {
            }
            virtual ~Group() throw()
            {
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Group);
        };
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(multi_map);
        
        DList dlist;
        DPool dpool;
        GPool gpool;
        
    };
}

#endif
