#ifndef YOCTO_ASSOCIATIVE_DUAL_MAP_INCLUDED
#define YOCTO_ASSOCIATIVE_DUAL_MAP_INCLUDED 1

#include "yocto/type/sub-key.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/hashing/sfh.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/core/list.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/code/htable.hpp"
#include "yocto/code/round.hpp"
#include "yocto/container/container.hpp"
#include "yocto/container/iter-linked.hpp"

namespace yocto
{
    namespace hidden
    {
        extern const char dual_map_name[];
    }
    
    //! a map indexed by two keys
    template <
    typename KEY,
    typename SUBKEY,
    typename T,
    typename HFUNC     = hashing::sfh,
    typename ALLOCATOR = memory::global::allocator >
    class dual_map : public container
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        YOCTO_ARGUMENTS_DECL_KEY;
        YOCTO_ARGUMENTS_DECL_SUBKEY;
        
        //! holds real keys
        class KNode
        {
        public:
            KNode       *next;
            KNode       *prev;
            const_key    key;
            const size_t hkey;
            const_subkey sub;
            const size_t hsub;
            T            data;
            
            inline KNode(param_key    k,
                         size_t       hk,
                         param_subkey s,
                         size_t       hs,
                         param_type   args) :
            next(0), prev(0), key(k), hkey(hk), sub(s), hsub(hs), data(args)
            {}
            
            inline ~KNode() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(KNode);
        };
        
        typedef memory::slab_of<KNode> KPool;
        typedef core::list_of<KNode>   KList;
        
        class HNode
        {
        public:
            HNode  *next;
            HNode  *prev;
            KNode  *knode;
            
            inline HNode(KNode *kn ) throw() : next(0), prev(0), knode(kn)
            { assert(knode); }
            
            inline ~HNode() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(HNode);
        };
        
        typedef core::list_of<HNode>   HSlot;
        typedef memory::slab_of<HNode> HPool;
        
        explicit dual_map() throw() :
        itmax(0),
        slots(0),
        klist(),
        kpool(0,0),
        keyTable(0),
        subTable(0),
        hpool(0,0),
        keyHasher(),
        subHasher(),
        wlen(),
        wksp(),
        hmem()
        {
            
            
        }
        
        explicit dual_map(size_t n, const as_capacity_t &):
        itmax(n),
        slots(htable::compute_slots_for(itmax)),
        klist(),
        kpool(0,0),
        keyTable(0),
        subTable(0),
        hpool(0,0),
        keyHasher(),
        subHasher(),
        wlen(),
        wksp(),
        hmem()
        {
            if(slots>0)
            {
                assert(itmax>0);
                //==============================================================
                // compute metrics
                //==============================================================
                const size_t kpool_length = KPool::bytes_for(itmax);
                const size_t k_tab_offset = memory::align(kpool_length);
                const size_t k_tab_length = itmax * sizeof(HSlot);
                const size_t s_tab_offset = memory::align(k_tab_offset+k_tab_length);
                const size_t s_tab_length = itmax * sizeof(HSlot);
                const size_t hpool_offset = memory::align(s_tab_offset+s_tab_length);
                const size_t hpool_nummax = 2*itmax;
                const size_t hpool_length = HPool::bytes_for(hpool_nummax);
                
                //==============================================================
                // acquire memory
                //==============================================================
                wlen = hpool_offset + hpool_length;
                wksp = hmem.acquire(wlen);
                uint8_t *addr = static_cast<uint8_t *>(wksp);
                
                //==============================================================
                // format memory
                //==============================================================
                kpool.format( &addr[0], itmax );
                keyTable = (HSlot *) &addr[k_tab_offset];
                subTable = (HSlot *) &addr[s_tab_offset];
                hpool.format( &addr[hpool_offset], hpool_nummax);
            }
        }
        
        
        virtual ~dual_map() throw() { __release(); }
        
        virtual const char *name() const throw()      { return hidden::dual_map_name; }
        virtual size_t      size() const throw()      { return klist.size; }
        virtual size_t      capacity() const throw()  { return itmax; }
        virtual void        free() throw()            { __free(); }
        virtual void        release() throw()         { __release(); }
        virtual void        reserve(size_t n) throw() { if(n>0) __reserve(n); }
        
        inline bool insert(param_key    key,
                           param_subkey sub,
                           param_type   args)
        {
            //==================================================================
            // dual check
            //==================================================================
            const size_t hkey = keyHasher(key);
            if(find_by_key(key,hkey))
                return false;
            
            const size_t hsub = subHasher(sub);
            if(find_by_sub(sub, hsub))
                return false;
            
            
            //==================================================================
            // memory check
            //==================================================================
            if( klist.size >= itmax )
                __reserve( next_increase(itmax) );
            
            //==================================================================
            // node insertion
            //==================================================================
            __insert(key, hkey, sub, hsub, args);
            return true;
            
        }
        
        inline bool has_key( param_key key ) const throw()
        {
            return 0 != find_by_key(key, keyHasher(key) );
        }
        
        inline bool has_sub( param_subkey sub ) const throw()
        {
            return 0 != find_by_sub(sub, subHasher(sub) );
        }
        
        inline const_subkey * get_sub_of( param_key key ) const throw()
        {
            const KNode *knode = find_by_key(key, keyHasher(key) );
            if(knode) return & knode->sub;
            return 0;
        }
        
        inline const_key * get_key_of( param_subkey sub ) const throw()
        {
            const KNode *knode = find_by_sub(sub, subHasher(sub) );
            if(knode) return & knode->key;
            return 0;
        }
        
        
        inline type * search( param_key key) throw()
        {
            KNode *knode = find_by_key(key, keyHasher(key) );
            if(knode) return & knode->data;
            return 0;
        }
        
        inline const_type * search( param_key key) const throw()
        {
            KNode *knode = find_by_key(key, keyHasher(key) );
            if(knode) return & knode->data;
            return 0;
        }
        
        inline const_type * sub_search( param_subkey sub) const throw()
        {
            KNode *knode = find_by_sub(sub, subHasher(sub) );
            if(knode) return & knode->data;
            return 0;
        }
        
        inline bool remove( param_key key ) throw()
        {
            KNode *knode =  find_by_key(key, keyHasher(key) );
            if(knode)
            {
                __remove(knode);
                return true;
            }
            return false;
        }
        
        inline bool sub_remove( param_subkey sub) throw()
        {
            KNode *knode =  find_by_sub(sub, subHasher(sub) );
            if(knode)
            {
                __remove(knode);
                return true;
            }
            return false;
        }
        
        
        inline void swap_with( dual_map &other ) throw()
        {
            cswap(itmax, other.itmax);
            cswap(slots, other.slots);
            mswap(klist, other.klist);
            kpool.swap_with(other.kpool);
            cswap(keyTable,other.keyTable);
            cswap(subTable,other.subTable);
            hpool.swap_with(other.hpool);
            cswap(wlen,other.wlen);
            cswap(wksp,other.wksp);
        }
        
        //======================================================================
		// iterators
		//======================================================================
		typedef iterating::linked<type,KNode,iterating::forward> iterator;
		inline iterator begin() throw() { return iterator( klist.head ); }
		inline iterator end()   throw() { return iterator( NULL );       }
		
		typedef iterating::linked<const_type,const KNode,iterating::forward> const_iterator;
		inline const_iterator begin() const throw() { return const_iterator( klist.head  ); }
		inline const_iterator end()   const throw() { return const_iterator( NULL );       }
        
        //======================================================================
		// auxiliary data
		//======================================================================
        inline size_t allocated_bytes() const throw() { return wlen;  }
        inline size_t num_slots() const throw()       { return slots; }
        
    private:
        //----------------------------------------------------------------------
        // metrics
        //----------------------------------------------------------------------
        size_t                        itmax; //!< max items in table
        size_t                        slots; //!< slots for htable metrics
        
        //----------------------------------------------------------------------
        // concrete dual keys node
        //----------------------------------------------------------------------
        KList                          klist; //!< list of KNodes
        KPool                          kpool; //!< pool of KNodes: capacity = count
        
        //----------------------------------------------------------------------
        // pointer to dual keys node
        //----------------------------------------------------------------------
        HSlot                          *keyTable; //!< indexed by key, #slots
        HSlot                          *subTable; //!< indexed by subkey, #slots
        HPool                           hpool; //!< 2 * max_items
        
        //----------------------------------------------------------------------
        // hashing functions
        //----------------------------------------------------------------------
        mutable key_hasher<KEY,HFUNC>    keyHasher;
        mutable key_hasher<SUBKEY,HFUNC> subHasher;
        
        //----------------------------------------------------------------------
        // memory provider
        //----------------------------------------------------------------------
        size_t                           wlen; //!< total #bytes allocated
        void                            *wksp; //!< memory
        ALLOCATOR                        hmem; //!< the allocator
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(dual_map);
        
        //======================================================================
        // find a concrete key node using the key/hkey
        //======================================================================
        inline KNode *find_by_key( param_key key, const size_t hkey ) const throw()
        {
            if(slots>0)
            {
                HSlot *hslot = (HSlot *)&keyTable[ hkey % slots ];
                for( HNode *h = hslot->head;h;h=h->next)
                {
                    if(h->knode->key==key)
                    {
                        assert(h->knode->hkey==hkey);
                        hslot->move_to_front(h);
                        return hslot->head->knode;
                    }
                }
            }
            return 0;
        }
        
        //======================================================================
        // find a concrete key node using the subkey/hsub
        //======================================================================
        inline KNode *find_by_sub( param_subkey sub, const size_t hsub ) const throw()
        {
            if(slots>0)
            {
                HSlot *hslot = (HSlot *)&subTable[ hsub % slots ];
                for( HNode *h = hslot->head;h;h=h->next)
                {
                    if(h->knode->sub==sub)
                    {
                        assert(h->knode->hsub==hsub);
                        hslot->move_to_front(h);
                        return hslot->head->knode;
                    }
                }
            }
            return 0;
        }
        
        //======================================================================
        // node expansion
        //======================================================================
        inline void __insert(param_key    key,
                             size_t       hkey,
                             param_subkey sub,
                             size_t       hsub,
                             param_type   args)
        {
            //==================================================================
            // create the key nodes
            //==================================================================
            assert(kpool.available()>=1);
            assert(hpool.available()>=2);
            assert(slots>0);
            
            //------------------------------------------------------------------
            // knode, may throw upon creating
            //------------------------------------------------------------------
            KNode *knode = kpool.query();
            try {
                new (knode) KNode(key,hkey,sub,hsub,args);
            }
            catch(...){ kpool.store(knode); throw; }
            
            //------------------------------------------------------------------
            // key/sub node, no throw
            //------------------------------------------------------------------
            HNode *key_node = new ( hpool.query() ) HNode(knode);
            HNode *sub_node = new ( hpool.query() ) HNode(knode);
            
            //==================================================================
            // put all the nodes in place
            //==================================================================
            klist.push_back(knode);
            keyTable[ hkey % slots ].push_front( key_node );
            subTable[ hsub % slots ].push_front( sub_node );
            
        }
        
        //======================================================================
        // subroutines to free all data
        //======================================================================
        inline void __free_hslot( HSlot &s ) throw()
        {
            while(s.size>0)
            {
                HNode *node = s.pop_back();
                destruct(node);
                hpool.store(node);
            }
        }
        
        inline void __free() throw()
        {
            for(size_t i=0; i < slots; ++i )
            {
                __free_hslot( keyTable[i] );
                __free_hslot( subTable[i] );
            }
            while(klist.size>0)
            {
                KNode *node = klist.pop_back();
                destruct(node);
                kpool.store(node);
            }
        }
        
        //======================================================================
        // release all resources
        //======================================================================
        inline void __release() throw()
        {
            __free();
            hpool.format(0,0);
            kpool.format(0,0);
            keyTable = 0;
            subTable = 0;
            slots    = 0;
            itmax    = 0;
            hmem.release(wksp,wlen);
        }
        
        //======================================================================
        // memory expansion
        //======================================================================
        inline void __reserve(size_t n)
        {
            assert(n>0);
            dual_map dm( itmax + n, as_capacity);
            __duplicate_into(dm);
            swap_with(dm);
        }
        
        inline void __duplicate_into( dual_map &dm ) const
        {
            assert( 0 == dm.size() );
            for( const KNode *node = klist.head; node; node=node->next)
            {
                dm.__insert(node->key,
                            node->hkey,
                            node->sub,
                            node->hsub,
                            node->data);
            }
        }
        
        //======================================================================
        // removal
        //======================================================================
        inline void __remove_from_hslot(HSlot       &hslot,
                                        const KNode *knode ) throw()
        {
            for( HNode *node = hslot.head; node; node=node->next)
            {
                if(node->knode==knode)
                {
                    node = hslot.unlink(node);
                    destruct(node);
                    hpool.store(node);
                    return;
                }
            }
            assert( die("never get here") );
        }
        
        inline void __remove( KNode *knode ) throw()
        {
            assert(knode);
            assert(klist.owns(knode));
            //------------------------------------------------------------------
            // unlink from list
            //------------------------------------------------------------------
            knode = klist.unlink(knode);
            
            //------------------------------------------------------------------
            // remove hnodes
            //------------------------------------------------------------------
            __remove_from_hslot( keyTable[knode->hkey%slots], knode );
            __remove_from_hslot( subTable[knode->hsub%slots], knode );
            
            //------------------------------------------------------------------
            // destruct and cache
            //------------------------------------------------------------------
            destruct(knode);
            kpool.store(knode);
            
        }
        
    };
    
}

#endif

