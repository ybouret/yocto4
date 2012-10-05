#ifndef CACHED_LIST_INCLUDED
#define CACHED_LIST_INCLUDED 1

#include "yocto/core/pool.hpp"
#include "yocto/object.hpp"

namespace yocto 
{
    
     
    //! cache of C++ destructed objects
    /**
     NODE must have an empty constructor
     */
    template <typename NODE>
    class cache_of 
    {
    public:
        //! create empty cache
        explicit cache_of() throw()  : pool()  {}
        
        //! create prefilled cache
        explicit cache_of( size_t n) : pool()
        {
            try { reserve(n);  }
            catch(...) { destroy(); }
        }
        virtual ~cache_of() throw() { destroy(); }
        
        //! destroy all cache
        inline void  destroy() throw() { while( pool.size ) object::release1<NODE>(pool.query()); }
        
        //! grow cache size
        inline void  reserve(size_t n) { while(n-- > 0) pool.store(  new_node() ); }
        
        //! build a new alive node
        inline NODE *provide()
        {
            NODE *node = pool.size > 0 ? pool.query() : new_node();
            try { new (node) NODE(); }
            catch(...) { pool.store(node); throw;  }
            return node;
        }
        
        //! duplicate another live node
        inline NODE *dup( const NODE *other )
        {
            NODE *node = pool.size > 0 ? pool.query() : new_node();
            try { new (node) NODE(*other); }
            catch(...) { pool.store(node); throw;  }
            return node;
        }
        
        inline void garbage( NODE *node ) throw()
        {
            assert(node);
            node->~NODE();
            pool.store(node);
        }
        
        inline size_t size() const throw() { return pool.size; }
        
    private:
        NODE *new_node() { return object::acquire1<NODE>(); }
        YOCTO_DISABLE_COPY_AND_ASSIGN(cache_of);
        core::pool_of<NODE> pool;
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
        
        //! call empty()
        virtual ~cached_list() throw() { empty(); }
        
        //! if NODE has a copy ctor
        cached_list( const cached_list &other ) :
        cache( other.cache )
        {
            try
            {
                for( const NODE *node = other.head; node; node=node->next )
                    this->push_back( cache.dup(node) );
            }
            catch(...) { empty(); throw; }
        }
        
        //! garbage all nodes
        inline void empty() throw()  { while( this->size ) cache.garbage( this->pop_back() );  }
        
        //! provide and push_back a node
        inline NODE *append()
        {
            NODE *node = cache.provide();
            assert(!this->owns(node));
            this->push_back(node);
            return node;
        }
        
        inline void append( size_t n )
        {
            while(n-- > 0) (void) append();
        }
        
        //! remove an cache any previously appended node
        inline void remove( NODE *node ) throw()
        {
            assert( 0 != node );
            assert( this->owns(node) );
            cache.garbage( this->unlink(node) );
        }
        
        //! garbage an unlinked node
        inline void garbage( NODE *node ) throw()
        {
            assert( 0 != node );
            assert( !this->owns(node) );
            cache.garbage(node);
        }
        
        cache_of<NODE> &cache;
        
    private:
        YOCTO_DISABLE_ASSIGN(cached_list);
    };
    
    
    
    
}


#endif
