#ifndef YOCTO_CORE_HANDLE_LIST_INCLUDED
#define YOCTO_CORE_HANDLE_LIST_INCLUDED 1

#include "yocto/core/cached-list.hpp"
#include "yocto/object.hpp"

namespace yocto 
{
    
    template <typename T>
    struct handle_of
    {
        class node_type : public object
        {
        public:
            explicit node_type() throw() : handle(0), next(0), prev(0) {}
            virtual ~node_type() throw() {}
            inline   node_type( const node_type &other ) throw() : 
            handle( other.handle ), next(0), prev(0) {}
            inline void reset() throw() { handle=0; }
            
            T         *handle;
            node_type *next;
            node_type *prev;
            
            
        private:
            YOCTO_DISABLE_ASSIGN(node_type);
        };

        typedef cache_of<node_type> cache_type;
        typedef cached_list<core::list_of,node_type> list_base;
        class list_type : public list_base
        {
        public:
            explicit list_type( cache_type &the_cache ) throw() :
            list_base(the_cache) {}
            inline   list_type( const list_type &other ) :
            list_base( other ) {}
            virtual ~list_type() throw() {}
            
            inline void attach( T *obj ) { this->append()->handle = obj; }
            
        private:
            YOCTO_DISABLE_ASSIGN(list_type);
        };
        
    };
    
        
        
}

#endif
