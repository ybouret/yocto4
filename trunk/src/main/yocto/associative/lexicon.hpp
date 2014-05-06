#ifndef YOCTO_LEXICON_INCLUDED
#define YOCTO_LEXICON_INCLUDED 1

#include "yocto/type/key.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/container/container.hpp"

namespace yocto
{
    
    namespace hidden
    {
        extern const char lexicon_name[];
    }
    
    //! each object IS a key
    /**
     The key part of each object MUST be constant...
     */
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global::allocator >
    class lexicon : public container
    {
    public:
        YOCTO_ASSOCIATIVE_KEY_T;
        static const size_t LOAD      = 4;
        static const size_t MIN_SLOTS = 4;
        
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
            
            inline void yield() throw()
            {
                assert(pool.size>0);
                object::release1( pool.query() );
            }
            
            inline size_t size() const throw() { return pool.size; }
            
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
            inline  HashNode( size_t h, DataNode *p ) throw() : next(0), prev(0), hkey(h), addr(p) { assert(p); }
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
            
            inline void yield() throw()
            {
                assert(pool.size>0);
                object::release1( pool.query() );
            }
            
            inline size_t size() const throw() { return pool.size; }
            
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
        
        inline virtual ~lexicon() throw() {  __release();  }
        
        inline virtual const char *name() const throw() { return hidden::lexicon_name; }
        inline virtual void        free() throw() { __free(); }
        inline virtual void        release() throw() { __release(); }
        inline virtual size_t      size()      const throw() { return ldata.size; }
        inline virtual size_t      capacity()  const throw() { return ldata.size + data_nodes.size(); }
        inline         size_t      num_slots() const throw() { return nslot; }
        inline virtual void        reserve(size_t n) { if(n>0) grow(n); }
        
        
        inline type * search( param_key key ) throw()
        {
            DataNode    *node = lookup(key);
            return node ? &(node->data) : 0;
        }
        
        inline const_type * search( param_key key ) const throw()
        {
            DataNode    *node = lookup(key);
            return node ? &(node->data) : 0;
        }
        
        
        
        inline bool insert( param_type args )
        {
            if( data_nodes.size() <= 0 )
            {
                grow( next_increase(size()) );
            }
            assert( data_nodes.size() > 0 );
            assert( data_nodes.size() == hash_nodes.size() );
            assert( nslot>0  );
            assert( slots!=0 );
            
            const_key   &akey  = args;
            const size_t hkey = hash(akey);
            HashSlot    &slot = slots[ hkey & hmask ];
            for(const HashNode *node = slot.head;node;node=node->next)
            {
                const DataNode *dn   = node->addr;
                if(dn->key()==akey)
                    return false;
            }
            
            // get a data node, copy args
            DataNode *dnode  = data_nodes.query(args);
            
            // get a hash node and put it in its slot
            try
            {
                HashNode *hnode  = hash_nodes.query(hkey,dnode);
                slot.push_front(hnode);
            }
            catch(...)
            {
                data_nodes.store(dnode);
                throw;
            }
            
            // put data in list
            ldata.push_back(dnode);
            
            return true;
        }
        
        
    private:
        core::list_of<DataNode>       ldata;  //!< key ordered data
        size_t                        nslot;  //!< a power of two >= data_list/LOAD;
        size_t                        hmask;  //!< num_slots-1
        HashSlot                     *slots;  //!< memory
        size_t                        count;  //!< for memory
        
        DataNodePool                  data_nodes;
        HashNodePool                  hash_nodes;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(lexicon);
        
        mutable KEY_HASHER            hash;
        ALLOCATOR                     hmem;
        
        inline void __free() throw()
        {
            for(size_t i=0;i<nslot;++i)
            {
                HashSlot &src = slots[i];
                while(src.size)
                {
                    hash_nodes.store(src.pop_back());
                }
            }
            while(ldata.size)
            {
                data_nodes.store( ldata.pop_back() );
            }
        }
        
        inline void __release() throw()
        {
            __free();
            data_nodes.clear();
            hash_nodes.clear();
            hmem.template release_as<HashSlot>(slots,count);
            nslot = 0;
            hmask = nslot-1;
        }
        
        //! reserve supplementary #nodes
        inline void reserve_nodes( size_t n )
        {
            assert(data_nodes.size()==hash_nodes.size());
            while(n-->0)
            {
                data_nodes.cache();
                try
                {
                    hash_nodes.cache();
                }
                catch(...)
                {
                    data_nodes.yield();
                    throw;
                }
            }
        }
        
        //! make new slots
        inline void rescale_slots( const size_t n )
        {
            // acquire new slots
            const size_t new_nslot = next_power_of_two( max_of<size_t>(n,MIN_SLOTS) );
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
        
        //
        inline DataNode *lookup(const_key &k) const throw()
        {
            if(nslot<=0)
            {
                return 0;
            }
            else
            {
                HashSlot &slot = slots[hash(k)&hmask];
                for(HashNode *node = slot.head;node;node=node->next)
                {
                    assert(node->addr);
                    DataNode *dn = node->addr;
                    if(dn->key() == k)
                    {
                        slot.move_to_front(node);
                        return dn;
                    }
                }
                return 0;
            }
        }
        
        static inline size_t __load_align( size_t n ) throw()
        {
            while(0!=(n%LOAD)) ++n;
            return n;
        }
        
        inline void grow(size_t n)
        {
            assert(n>0);
            assert(LOAD*nslot>=capacity());
            assert(data_nodes.size() == hash_nodes.size());
            const size_t num_nodes = data_nodes.size();
            try
            {
                reserve_nodes(n);
                const size_t new_cap   = capacity();
                const size_t opt_cap   = __load_align(new_cap);
                const size_t new_nslot = max_of<size_t>(opt_cap/LOAD,MIN_SLOTS);
                if(new_nslot>nslot)
                    rescale_slots(new_nslot);
                assert(nslot>=capacity()/LOAD);
            }
            catch(...)
            {
                while( data_nodes.size() >= num_nodes ) data_nodes.yield();
                while( hash_nodes.size() >= num_nodes ) hash_nodes.yield();
                throw;
            }
        }
        
    };
    
    
}

#endif
