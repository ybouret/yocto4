#ifndef CACHED_LIST_INCLUDED
#define CACHED_LIST_INCLUDED 1

#include "yocto/core/pool.hpp"

namespace yocto 
{
    
     
    //! cache of C++ objects
    /**
     NODE must have an empty constructor
     and a nothrow reset method.
     */
    template <typename NODE>
    class cache_of : public core::pool_of<NODE>
    {
    public:
        //! create empty cache
        explicit cache_of() throw()  : core::pool_of<NODE>() {}
        
        //! create prefilled cache
        explicit cache_of( size_t n) : core::pool_of<NODE>() 
        {
            try { reserve(n);  }
            catch(...) { destroy(); }
        }
        virtual ~cache_of() throw() { destroy(); }
        
        //! destroy all cache
        inline void  destroy() throw() { while( this->size ) delete this->query(); }
        
        //! grow cache size
        inline void  reserve(size_t n) { while(n-- > 0) this->store( new NODE() ); }
        
        //! a new/reset node
        /**
         warning: query() shouldn't be used directly
         */
        inline NODE *provide()
        {
            if( this->size > 0 )
            {
                NODE *node = this->query();
                node->reset();
                return node;
            }
            else 
            {
                return new NODE();
            }
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(cache_of);
    };
    
    //! a list with a public shared cache
    template <template <typename> class LIST_OF,typename NODE>
    class cached_list : public LIST_OF<NODE>
    {
    public:
        //! use an external cache
        explicit cached_list( cache_of<NODE> &the_cache ) throw() :
        cache( the_cache )
        {
        }
        
        //! delete all its nodes
        /**
         use empty() before to preserve cache
         */
        virtual ~cached_list() throw()
        {
            while( this->size ) delete this->pop_back();
        }
        
        //! if NODE has a copy ctor
        cached_list( const cached_list &other ) :
        cache( other.cache )
        {
            try
            {
                for( const NODE *node = other.head; node; node=node->next )
                    this->push_back( new NODE(*node) );
            }
            catch(...) { empty(); throw; }
        }
        
        inline void empty() throw()
        {
            while( this->size ) cache.store( this->pop_back() );
        }
        
        //! query and push_back a node
        inline NODE *append()
        {
            NODE *node = cache.provide();
            assert(!this->owns(node));
            this->push_back(node);
            return node;
        }
        
        //! remove an cache any previously appended node
        inline void remove( NODE *node ) throw()
        {
            assert( 0 != node );
            assert( this->owns(node) );
            cache.store( this->unlink(node) );
        }
        
        cache_of<NODE> &cache;
        
    private:
        YOCTO_DISABLE_ASSIGN(cached_list);
    };
    
    
    
    
}


#endif
