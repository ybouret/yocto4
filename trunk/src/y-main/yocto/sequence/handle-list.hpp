#ifndef YOCTO_CORE_HANDLE_LIST_INCLUDED
#define YOCTO_CORE_HANDLE_LIST_INCLUDED 1

#include "yocto/sequence/cached-list.hpp"
#include "yocto/core/list.hpp"

namespace yocto 
{
    
        
    //! HANDLE_NODE must have a handle member
    template <typename HANDLE_NODE>
    class handle_list : public cached_list<core::list_of,HANDLE_NODE>
    {
    public:
        explicit handle_list( cache_of<HANDLE_NODE> &the_cache ) throw() :
        cached_list<core::list_of,HANDLE_NODE>( the_cache ) {}
        
        virtual ~handle_list() throw() {}
        
        inline   handle_list( const handle_list &other ) :
        cached_list<core::list_of,HANDLE_NODE>(other) {}
        
        template <typename T>
        inline void attach( T *obj ) { this->append()->handle = obj; }
        
    private:
        YOCTO_DISABLE_ASSIGN(handle_list);
    };
    
        
        
}

#endif
