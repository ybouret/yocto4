#ifndef YOCTO_ASSOC_MULTI_MAP_INCLUDED
#define YOCTO_ASSOC_MULTI_MAP_INCLUDED

#include "yocto/container/container.hpp"
#include "yocto/type/key.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"

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
        
        class Group : public object, public core::list_of<GNode>
        {
        public:
            const_key    key;
            const size_t hkey;
            Group       *prev;
            Group       *next;
            
            explicit Group( param_key k, const size_t h) :
            key(k),
            hkey(h),
            prev(0),
            next(0)
            {
            }
            virtual ~Group() throw()
            {
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Group);
        };
        
        typedef core::list_of<Group> GSlot;
        typedef core::pool_of<Group> GPool;
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(multi_map);
        
        DList dlist;
        DPool dpool;
        
        
        
    };
}

#endif
