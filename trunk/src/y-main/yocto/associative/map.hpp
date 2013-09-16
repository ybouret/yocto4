#ifndef YOCTO_ASSOCIATIVE_MAP_INCLUDED
#define YOCTO_ASSOCIATIVE_MAP_INCLUDED 1

#include "yocto/container/associative.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/htable.hpp"
#include "yocto/core/list.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/container/iter-linked.hpp"

namespace yocto
{
	
	namespace hidden
	{
		extern const char map_name[];
	}
    
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global::allocator >
	class map : public associative<KEY,T>
	{
    public:
        YOCTO_ASSOCIATIVE_KEY_T;
        
        //! key/hkey/data
        class KNode
        {
        public:
            KNode       *prev;
            KNode       *next;
            const_key    key;
            const size_t hkey;
            type         data;
            
            inline KNode( param_key k, size_t h, param_type args) :
            prev(0), next(0), key(k), hkey(h), data(args) {}
            inline ~KNode() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(KNode);
        };
        
        typedef core::list_of<KNode>   KList;
        typedef memory::slab_of<KNode> KPool;
        
        
        //! handle node on knode
        class HNode
        {
        public:
            HNode *prev;
            HNode *next;
            KNode *knode;
            inline  HNode(KNode *kn) throw() :
            prev(0), next(0), knode(kn) { assert(knode); }
            inline ~HNode() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(HNode);
        };
        
        typedef core::list_of<HNode>    HSlot;
        typedef memory::slab_of<HNode>  HPool;
        
        //! default ctor
        explicit map() throw() :
        itmax(0),
        slots(0),
        klist(),
        kpool(0,0),
        hslot(0),
        hpool(0,0),
        wlen(0),
        wksp(0),
        hash(),
        hmem()
        {
        }
        
        //! prepare for capacity
        explicit map( size_t n, const as_capacity_t & ) :
        itmax(n),
        slots( htable::compute_slots_for(itmax) ),
        klist(),
        kpool(0,0),
        hslot(0),
        hpool(0,0),
        wlen(0),
        wksp(0),
        hash(),
        hmem()
        {
            __init();
        }
        
        //! copy ctor
        explicit map( const map &other ) :
        itmax(other.size()),
        slots(htable::compute_slots_for(itmax)),
        klist(),
        kpool(0,0),
        hslot(0),
        hpool(0,0),
        wlen(0),
        wksp(0),
        hash(),
        hmem()
        {
            __init();
            try { other.__duplicate_into( *this ); }
            catch(...) {__release(); throw;        }
        }

        
        //! no-throw swap
        inline void swap_with( map &other ) throw()
        {
            cswap(itmax, other.itmax);
            cswap(slots, other.slots);
            klist.swap_with(other.klist);
            kpool.swap_with(other.kpool);
            cswap(hslot,other.hslot);
            hpool.swap_with(other.hpool);
            cswap(wlen,other.wlen);
            cswap(wksp,other.wksp);
        }
        
        virtual ~map() throw() { __release(); }
        
        
        virtual const char *name() const throw()     { return hidden::map_name; }
        virtual void        free() throw()           { __free(); }
        virtual void        release()  throw()       { __release(); }
        virtual size_t      size()     const throw() { return klist.size; }
        virtual size_t      capacity() const throw() { return itmax; }
        virtual void        reserve(size_t n)
        {
            if(n>0)
            {
                map tmp( itmax +n, as_capacity );
                __duplicate_into(tmp);
                swap_with(tmp);
            }
        }
        
        virtual bool insert( param_key key, param_type args )
        {
            //------------------------------------------------------------------
            // check key
            //------------------------------------------------------------------
            HSlot       *slot = 0;
            const size_t hkey = hash(key);
            if( (slots>0) && (0!=__find(key, hkey, slot)) ) return false;
            
            //------------------------------------------------------------------
            // check memory/insert
            //------------------------------------------------------------------
            if( klist.size >= itmax )
            {
                map tmp( container::next_capacity(itmax), as_capacity );
                __duplicate_into(tmp);
                tmp.__insert(key,hkey,args);
                swap_with(tmp);
            }
            else
            {
                __insert(key,hkey,args);
            }
            return true;
        }
        
        
        virtual bool remove( param_key key ) throw()
        {
            const size_t hkey = hash(key);
            if(slots>0)
            {
                HSlot *slot = 0;
                HNode *node = __find(key,hkey,slot);
                if(node)
                {
                    assert(slot);
                    assert(slot->head==node);
                    KNode *knode = node->knode;
                    destruct( klist.unlink(knode) );
                    kpool.store(knode);
                    hpool.store( slot->pop_front() );
                    return true;
                }
            }
            return false;
        }
        
        //======================================================================
		// iterators
		//======================================================================
		typedef iterating::linked<type,KNode,iterating::forward> iterator;
		inline iterator begin() throw() { return iterator(klist.head); }
		inline iterator end()   throw() { return iterator(0);          }
        
        typedef iterating::linked<type,KNode,iterating::reverse> reverse_iterator;
		inline reverse_iterator rbegin() throw() { return reverse_iterator(klist.tail); }
		inline reverse_iterator rend()   throw() { return reverse_iterator(0);          }
		
		typedef iterating::linked<const_type,const KNode,iterating::forward> const_iterator;
		inline const_iterator begin() const throw() { return const_iterator(klist.head); }
		inline const_iterator end()   const throw() { return const_iterator(0);          }
     
        typedef iterating::linked<const_type,const KNode,iterating::reverse> const_reverse_iterator;
		inline const_reverse_iterator rbegin() const throw() { return const_reverse_iterator(klist.head); }
		inline const_reverse_iterator rend()   const throw() { return const_reverse_iterator(0);          }

        //======================================================================
        // signature
        //======================================================================
        size_t signature() const throw()
        {
            hashing::sha1 H;
            H.set();
            for(const KNode *knode=klist.head;knode;knode=knode->next)
            {
                H.run_type(knode->hkey);
            }
            return H.key<size_t>();
        }
        
    private:
        //----------------------------------------------------------------------
        // metrics
        //----------------------------------------------------------------------
        size_t                        itmax; //!< max items in table
        size_t                        slots; //!< slots for htable metrics
        
        //----------------------------------------------------------------------
        // holding key/data
        //----------------------------------------------------------------------
        KList                         klist;
        KPool                         kpool;
        
        //----------------------------------------------------------------------
        // holding slots
        //----------------------------------------------------------------------
        mutable HSlot                *hslot; // 0..slots
        HPool                         hpool;
        
        //----------------------------------------------------------------------
        // memory
        //----------------------------------------------------------------------
        size_t                        wlen;
        void                         *wksp;
        
        //----------------------------------------------------------------------
        // components
        //----------------------------------------------------------------------
        mutable KEY_HASHER            hash;
        ALLOCATOR                     hmem;
        
        YOCTO_DISABLE_ASSIGN(map);
        
        //======================================================================
		// free
		//======================================================================
        inline void __free_slot(HSlot &slot) throw()
        {
            while(slot.size>0)
            {
                HNode *node = slot.pop_back();
                destruct(node);
                hpool.store(node);
            }
        }
        
        inline void __free() throw()
        {
            for(size_t i=0;i<slots;++i) __free_slot( hslot[i] );
            while(klist.size)
            {
                KNode *knode = klist.pop_back();
                destruct(knode);
                kpool.store(knode);
            }
        }
        
        //======================================================================
		// release
		//======================================================================
        inline void __release() throw()
        {
            __free();
            hmem.release(wksp,wlen);
            slots = 0;
            itmax = 0;
            hslot = 0;
            kpool.format(0,0);
            hpool.format(0,0);
        }
        
        //======================================================================
        // look up, virtual for associative interface
        //======================================================================
        inline HNode * __find( param_key key, const size_t hkey, HSlot * &slot ) const throw()
        {
            assert(slots);
            assert(hslot);
            assert(0==slot);
            slot = &hslot[ hkey % slots ];
            for(HNode *node=slot->head;node;node=node->next)
            {
                assert(node->knode);
                if(key == node->knode->key)
                {
                    slot->move_to_front(node);
                    return node;
                }
            }
            return 0;
        }
        
        virtual const_type *lookup( param_key key ) const throw()
        {
            if(slots)
            {
                HSlot *slot = 0;
                HNode *node = __find(key,hash(key),slot);
                if(node) return & node->knode->data;
            }
            return 0;
        }
        
        
        //======================================================================
        // memory init
        //======================================================================
        inline void __init()
        {
            if(slots>0)
            {
                const size_t kpool_offset = 0;
                const size_t kpool_length = KPool::bytes_for(itmax);
                const size_t hslot_offset = memory::align(kpool_offset + kpool_length);
                const size_t hslot_length = sizeof(HSlot) * slots;
                const size_t hpool_offset = memory::align(hslot_offset + hslot_length);
                const size_t hpool_length = HPool::bytes_for(itmax);
                
                wlen = hpool_offset + hpool_length;
                wksp = hmem.acquire(wlen);
                
                uint8_t *p = static_cast<uint8_t *>(wksp);
                kpool.format( &p[kpool_offset], itmax);
                hslot      = (HSlot *) &p[hslot_offset];
                hpool.format( &p[hpool_offset], itmax);
            }
        }
        
        //======================================================================
        // insertion
        //======================================================================
        inline void __insert( param_key key, const size_t hkey, param_type args )
        {
            assert(klist.size<itmax);
            assert(kpool.available()>0);
            assert(hpool.available()==kpool.available());
            assert(slots);
            assert(hslot);
            
            KNode *knode = kpool.query();
            try {
                new (knode) KNode(key,hkey,args);
            }
            catch(...)
            {
                kpool.store(knode); throw;
            }
            HNode *node = new ( hpool.query() ) HNode(knode);
            hslot[ hkey % slots ].push_front(node);
            klist.push_back(knode);
        }
        
        //======================================================================
        // duplication
        //======================================================================
        inline void __duplicate_into( map &other ) const
        {
            assert(other.size()==0);
            assert(other.capacity()>=this->size());
            
            for( const KNode *knode = klist.head; knode; knode=knode->next)
            {
                other.__insert( knode->key, knode->hkey, knode->data);
            }
        }
    };
    

}

#endif
