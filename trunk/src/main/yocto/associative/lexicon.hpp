#ifndef YOCTO_LEXICON_INCLUDED
#define YOCTO_LEXICON_INCLUDED 1

#include "yocto/type/key.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"

namespace yocto
{
    //! each object IS a key
    /**
     The key part of each object MUST be constant...
     */
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global::allocator >
    class lexicon
    {
    public:
        YOCTO_ASSOCIATIVE_KEY_T;
        
        class DataNode
        {
        public:
            DataNode     *next;
            DataNode     *prev;
            mutable_type  data;
            inline const_key & key() const throw() { return data; }
            
            inline  DataNode( param_type args ) : next(0), prev(0), data(args) {}
            inline ~DataNode() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DataNode);
        };
        
        class DataNodePool
        {
        public:
            inline  DataNodePool() throw() {}
            inline ~DataNodePool() throw() { clear(); }
            
            inline void clear() throw() { while(pool.size) object::release1( pool.query() ); }
            inline void store( DataNode *alive ) throw()
            {
                assert(alive);
                alive->data.~mutable_type();
                pool.store(alive);
            }
            
            inline DataNode *query( param_type args )
            {
                DataNode *node = pool.size ? pool.query() : object::acquire1<DataNode>();
                try
                {
                    new (&(node->data)) mutable_type(args);
                }
                catch(...)
                {
                    pool.store(node);
                    throw;
                }
                return node;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DataNodePool);
            core::pool_of<DataNode> pool;
        };
        
        
        
        class HashNode
        {
        public:
            HashNode    *next;
            HashNode    *prev;
            const size_t hkey;
            DataNode    *addr;
            inline  HashNode( size_t h, void *p ) throw() : next(0), prev(0), hkey(h), addr(p) { assert(p); }
            inline ~HashNode() throw() {}
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(HashNode);
        };
        
        class HashNodePool
        {
        public:
            inline  HashNodePool() throw() : pool() {}
            inline ~HashNodePool() throw() {clear();}
            
            inline void clear() throw() { while(pool.size) object::release1(pool.query()); }
            inline void store(HashNode *node) throw()
            {
                assert(node);
                node->addr = 0;
                pool.store(node);
            }
            
            inline HashNode *query( size_t hkey, DataNode *addr )
            {
                assert(addr);
                return new ( pool.size ? pool.query() : object::acquire1<HashNode>() ) HashNode(hkey,addr);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(HashNodePool);
            core::pool_of<HashNode> pool;
        };
        
        typedef core::list_of<HashNode> HashSlot;
        
        
    private:
        core::list_of<DataNode>       data_list;
        
        
    public:
        DataNodePool                  data_nodes;
        core::pool_of<HashNode>       hash_nodes;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(lexicon);
       
        mutable KEY_HASHER            hash;
        ALLOCATOR                     hmem;
    };
    
    
}

#endif
