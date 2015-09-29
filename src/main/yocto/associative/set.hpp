#ifndef YOCTO_ASSOCIATIVE_SET_INCLUDED
#define YOCTO_ASSOCIATIVE_SET_INCLUDED 1


#include "yocto/container/sequence.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/htable.hpp"
#include "yocto/core/list.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/container/iter-linked.hpp"
#include "yocto/type/key.hpp"
#include "yocto/sort/merge.hpp"

namespace yocto
{
	namespace hidden
	{
		extern const char set_name[];
	}
	
    //! set of object with a key() method, MUST BE A REFERENCE !
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global::allocator >
	class set : public container
	{
    public:
        YOCTO_ASSOCIATIVE_KEY_T;
        
        //! key/hkey/data
        class KNode
        {
        public:
            KNode       *prev;
            KNode       *next;
            const size_t hkey;
            type         data;
            
            inline KNode(  size_t h, param_type args) :
            prev(0), next(0),hkey(h), data(args) {}
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
        explicit set() throw() :
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
        explicit set( size_t n, const as_capacity_t & ) :
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
        explicit set( const set &other ) :
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
        inline void swap_with( set &other ) throw()
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
        
        virtual ~set() throw() { __release(); }
        
        
        virtual const char *name() const throw()     { return hidden::set_name; }
        virtual void        free() throw()           { __free(); }
        virtual void        release()  throw()       { __release(); }
        virtual size_t      size()     const throw() { return klist.size; }
        virtual size_t      capacity() const throw() { return itmax; }
        virtual void        reserve(size_t n)
        {
            if(n>0)
            {
                set tmp( itmax +n, as_capacity );
                __duplicate_into(tmp);
                swap_with(tmp);
            }
        }
        
        inline bool insert(param_type args)
        {
            //------------------------------------------------------------------
            // check key
            //------------------------------------------------------------------
            HSlot       *slot = 0;
            const size_t hkey = hash(args.key());
            if( (slots>0) && (0!=__find( args.key(), hkey, slot)) ) return false;
            
            //------------------------------------------------------------------
            // check memory/insert
            //------------------------------------------------------------------
            __check_and_insert(hkey, args);
            return true;
        }
        
        inline void insert__(param_type args)
        {
            assert(0==search( args.key() ));
            __check_and_insert( hash(args.key()), args);
        }
        
        
        inline bool remove( param_key key ) throw()
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
        
        inline type *search(param_key key) throw()
        {
            if(slots)
            {
                HSlot * slot = 0;
                HNode * node = __find(key, hash(key), slot);
                if(node) return & node->knode->data;
            }
            return 0;
        }
        
        inline const_type *search(param_key key) const throw()
        {
            if(slots)
            {
                HSlot * slot = 0;
                HNode * node = __find(key, hash(key), slot);
                if(node) return & node->knode->data;
            }
            return 0;
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
        // fast access
        //======================================================================
        inline type       &front()       throw() { assert(klist.head); return klist.head->data; }
        inline const_type &front() const throw() { assert(klist.head); return klist.head->data; }
        inline type       &back()        throw() { assert(klist.head); return klist.tail->data; }
        inline const_type &back()  const throw() { assert(klist.head); return klist.tail->data; }
        
        //======================================================================
        // slow access
        //======================================================================
        inline type & operator()(size_t indx) throw()
        {
            assert(indx>0);
            assert(indx<=this->size());
            return klist.fetch(--indx)->data;
        }
        
        inline const_type & operator()(size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=this->size());
            return ((KList &)klist).fetch(--indx)->data;
        }
        
        //======================================================================
        // signature
        //======================================================================
        inline size_t signature() const throw()
        {
            hashing::sha1 H;
            H.set();
            for(const KNode *knode=klist.head;knode;knode=knode->next)
            {
                H.run_type(knode->hkey);
            }
            return H.key<size_t>();
        }

        //======================================================================
        // ordering
        //======================================================================
        typedef int (*data_compare)(const_type &lhs, const_type &rhs);
        
        static inline int node_compare( const KNode *lhs, const KNode *rhs, void *args ) throw()
        {
            assert(lhs);
            assert(rhs);
            assert(args);
            
            data_compare proc  = (data_compare)args;
            return proc(lhs->data,rhs->data);
        }
        
        inline void sort_by( data_compare proc )
        {
            core::merging<KNode>::sort(klist,node_compare,(void*)proc);
        }

        //======================================================================
        //! in place removing
        //======================================================================
        template <typename FUNC>
        inline void remove_if( FUNC &is_bad ) throw()
        {
            HSlot bad;

            //__________________________________________________________________
            //
            // collect bad
            //__________________________________________________________________
            for(size_t i=0;i<slots;++i)
            {
                HSlot  tmp;
                HSlot &s = hslot[i];
                while(s.size>0)
                {
                    HNode *node = s.pop_back();
                    T     &data = node->knode->data;
                    if(is_bad(data))
                    {
                        bad.push_back(node);
                    }
                    else
                    {
                        tmp.push_front(node);
                    }
                }
                s.swap_with(tmp);
            }

            //__________________________________________________________________
            //
            // get rid of bad
            //__________________________________________________________________
            while(bad.size>0)
            {
                HNode *node  = bad.pop_back();
                KNode *knode = node->knode;
                destruct(node);
                hpool.store(node);

                destruct(klist.unlink(knode));
                kpool.store(knode);
            }
        }

        inline void collect_keys( sequence<KEY> &key_seq ) const
        {
            for(const KNode *node = klist.head; node; node=node->next )
            {
                key_seq.push_back(node->data.key());
            }
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
    public:
        mutable KEY_HASHER            hash;
        ALLOCATOR                     hmem;
        
    private:
        YOCTO_DISABLE_ASSIGN(set);
        
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
        inline HNode * __find(param_key    key,
                              const size_t hkey,
                              HSlot *     &slot ) const throw()
        {
            assert(slots);
            assert(hslot);
            assert(0==slot);
            slot = &hslot[ hkey % slots ];
            for(HNode *node=slot->head;node;node=node->next)
            {
                assert(node->knode);
                if(key == node->knode->data.key())
                {
                    slot->move_to_front(node);
                    return node;
                }
            }
            return 0;
        }
        
        inline const_type *lookup( param_key key ) const throw()
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
        inline void __check_and_insert(const size_t hkey, param_type args )
        {
            if( klist.size >= itmax )
            {
                set tmp( container::next_capacity(itmax), as_capacity );
                __duplicate_into(tmp);
                tmp.__insert(hkey,args);
                swap_with(tmp);
            }
            else
            {
                __insert(hkey,args);
            }
        }
        
        inline void __insert( const size_t hkey, param_type args )
        {
            assert(klist.size<itmax);
            assert(kpool.available()>0);
            assert(hpool.available()==kpool.available());
            assert(slots);
            assert(hslot);
            
            KNode *knode = kpool.query();
            try {
                new (knode) KNode(hkey,args);
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
        inline void __duplicate_into( set &other ) const
        {
            assert(other.size()==0);
            assert(other.capacity()>=this->size());
            
            for( const KNode *knode = klist.head; knode; knode=knode->next)
            {
                other.__insert(knode->hkey, knode->data);
            }
        }
    };
    
}

#endif
