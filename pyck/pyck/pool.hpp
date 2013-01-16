#ifndef WINK_POOL_INCLUDED
#define WINK_POOL_INCLUDED 1


#include "./os.hpp"


//! core pool
template <typename NODE>
class PoolOf
{
public:
    //! default constructor
    inline explicit PoolOf() throw() : top(NULL), size(0) {}
    
    //! destructor: pool must be empty
    inline virtual ~PoolOf() throw() { assert(NULL==top); assert(0==size); }
    
    NODE  *top;  //!< top of pool
    size_t size; //!< nodes in pool
    
    
    //! push a valid node
    inline void store( NODE *node ) throw()
    {
        assert( NULL != node       );
        assert( NULL == node->next );
        assert( ! owns(node)       );
        node->next = top;
        top        = node;
        ++size;
    }
    
    //! query is size>0
    inline NODE *query() throw()
    {
        assert(top != NULL);
        assert(size > 0   );
        NODE *node = top;
        top = top->next;
        node->next = NULL;
        --size;
        return node;
    }
    
    //! linear search
    inline bool owns( const NODE *node ) const throw()
    {
        for( const NODE *scan = top; scan != NULL; scan = scan->next )
        {
            if( scan == node ) return true;
        }
        return false;
    }
    
    
    inline void delete_with( void (*proc)(NODE *) ) throw()
    {
        assert( proc );
        while(size>0) proc( query() );
    }
    
    
    template <typename ARGS>
    inline void delete_with( void (*proc)( NODE *, ARGS), ARGS args ) throw()
    {
        assert(proc);
        while(size>0) proc( query(), args );
    }
    
    
    inline void reset() throw() { top = NULL; size = 0; }
    
    inline void reverse() throw()
    {
        PoolOf<NODE> tmp;
        while( size ) tmp.store( query() );
        cswap(size,tmp.size);
        cswap(top, tmp.top );
    }
    
private:
    PoolOf(const PoolOf&);
    PoolOf&operator=(const PoolOf&);
};



#endif
