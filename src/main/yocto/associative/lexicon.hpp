#ifndef YOCTO_LEXICON_INCLUDED
#define YOCTO_LEXICON_INCLUDED 1

#include "yocto/type/key.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/code/utils.hpp"

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
        static const size_t LOAD = 4;
        //======================================================================
        // dynamic node to store data
        //======================================================================
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
            
            inline void cache()
            {
                pool.store( object::acquire1<DataNode>() );
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DataNodePool);
            core::pool_of<DataNode> pool;

        };
        
        
        //======================================================================
        // dynamic node to store hash key
        //======================================================================
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
            
            inline void cache()
            {
                pool.store( object::acquire1<HashNode>() );
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(HashNodePool);
            core::pool_of<HashNode> pool;
        };
        
        typedef core::list_of<HashNode> HashSlot;
        
        //======================================================================
        // API
        //======================================================================
        inline explicit lexicon() throw() :
        ldata(),
        nslot(0),
        hmask(nslot-1),
        slots(0),
        count(0),
        hash(),
        hmem()
        {
        }
        
        inline virtual ~lexicon() throw()
        {
            __release();
        }
        
        inline void free() throw() { __free(); }
        
        inline type * search( param_key k )
        {
            const size_t h    = hash(k);
            HashSlot    *slot = 0;
            DataNode    *node = lookup(h, k, slot);
            if(node)
            {
                return & (node->data);
            }
            else
            {
                return 0;
            }
        }
        
        
        
    private:
        core::list_of<DataNode>       ldata;  //!< key ordered data
        size_t                        nslot;  //!< a power of two >= data_list/LOAD;
        size_t                        hmask;  //!< num_slots-1
        HashSlot                     *slots;  //!< memory
        size_t                        count;  //!< for memory
        
        DataNodePool                  data_nodes;
        core::pool_of<HashNode>       hash_nodes;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(lexicon);
        
        mutable KEY_HASHER            hash;
        ALLOCATOR                     hmem;
        
        inline void __free() throw()
        {
            for(size_t i=0;i<nslot;++i)
            {
                HashSlot &src = slots[i];
                while(src.size) hash_nodes.store(src.pop_back());
            }
            while(ldata.size)
            {
                data_nodes.store( ldata.pop_back() );
            }
        }
        
        inline void __release() throw()
        {
            __free();
            hmem.template release_as<HashSlot>(slots,count);
            nslot = 0;
            hmask = nslot-1;
        }
        
        //! reserve supplementary #nodes
        inline void reserve_nodes( size_t n )
        {
            assert(data_nodes.pool.size==hash_nodes.pool.size);
            while(n-->0)
            {
                {
                    data_nodes.cache();
                }
                try
                {
                    hash_nodes.cache();
                }
                catch(...)
                {
                    object::release1( data_nodes.pool.query() );
                    throw;
                }
            }
        }
        
        inline void rescale_slots( const size_t n )
        {
            // acquire new slots
            const size_t new_nslot = next_power_of_two( max_of<size_t>(n,16) );
            size_t       new_count = new_nslot;
            HashSlot    *new_slots = hmem.template acquire_as<HashSlot>(new_count);
            const size_t new_hmask = new_nslot - 1;
            
            // transfert nodes
            for(size_t i=0;i<nslot;++i)
            {
                HashSlot &src = slots[i];
                while(src.size)
                {
                    HashNode *node = src.pop_front();
                    HashSlot &tgt  = new_slots[ node->hkey & new_hmask ];
                    tgt.push_back(node);
                }
            }
            
            // exchange memory
            hmem.template release_as<HashSlot>(slots,count);
            nslot = new_nslot;
            slots = new_slots;
            count = new_count;
            hmask = new_hmask;
        }
        
        inline DataNode *lookup( const size_t h, const_key &k, HashSlot * &slot ) const throw()
        {
            assert(0==slot);
            if(nslot<=0)
            {
                return 0;
            }
            else
            {
                slot = &slots[ h & hmask ];
                for( const HashNode *node = slot->head;node;node=node->next)
                {
                    assert(node->addr);
                    const DataNode *dn = node->addr;
                    if(dn->key() == k)
                    {
                        return (DataNode *)dn;
                    }
                }
                return 0;
            }
        }
        
        
    };
    
    
}

#endif
