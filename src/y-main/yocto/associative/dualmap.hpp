#ifndef YOCTO_ASSOCIATIVE_DUAL_MAP_INCLUDED
#define YOCTO_ASSOCIATIVE_DUAL_MAP_INCLUDED 1

#include "yocto/type/sub-key.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/hashing/sfh.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/core/list.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/code/htable.hpp"

namespace yocto
{
    namespace hidden
    {
        extern const char dualmap_name[];
    }
    
    template <
    typename KEY,
    typename SUBKEY,
    typename T,
    typename HFUNC     = hashing::sfh,
    typename ALLOCATOR = memory::global::allocator >
    class dualmap
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
        
        explicit dualmap() throw() :
        itmax(0),
        slots(0),
        count(0),
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
        
        virtual ~dualmap() throw() {}
        
        virtual const char *name() const throw() { return hidden::dualmap_name; }
        
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
            
            //==================================================================
            // create the key nodes
            //==================================================================
            assert(kpool.available()>=1);
            assert(hpool.available()>=2);
            assert(slots>0);
            
            //------------------------------------------------------------------
            // knode
            //------------------------------------------------------------------
            KNode *knode = kpool.query();
            try {
                new (knode) KNode(key,hkey,sub,hsub,args);
            }
            catch(...){ kpool.store(knode); throw; }
            
            //------------------------------------------------------------------
            // key/sub node
            //------------------------------------------------------------------
            HNode *key_node = new ( hpool.query() ) HNode(knode);
            HNode *sub_node = new ( hpool.query() ) HNode(knode);
            
            //==================================================================
            // put all the nodes in place
            //==================================================================
            klist.push_back(knode);
            keyTable[ hkey % slots ].push_front( key_node );
            subTable[ hsub % slots ].push_front( sub_node );
            return true;
            
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
        
    private:
        //----------------------------------------------------------------------
        // metrics
        //----------------------------------------------------------------------
        size_t                        itmax; //!< max items in table
        size_t                        slots; //!< slots for htable metrics
        size_t                        count; //!< currently in tables
        
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
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(dualmap);
        
        //======================================================================
        // find a concrete key node using the key/hkey
        //======================================================================
        KNode *find_by_key( param_key key, const size_t hkey ) const throw()
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
        KNode *find_by_sub( param_subkey sub, const size_t hsub ) const throw()
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
        
        
    };
    
}

#endif

