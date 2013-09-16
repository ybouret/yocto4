#ifndef YOCTO_ASSOCIATIVE_MAP_INCLUDED
#define YOCTO_ASSOCIATIVE_MAP_INCLUDED 1

#include "yocto/container/associative.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/htable.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/core/list.hpp"
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
	class map : public container
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
                
            }
        }
        
        virtual bool insert( param_key key, param_type args )
        {
            //------------------------------------------------------------------
            // check key
            //------------------------------------------------------------------
            const size_t hkey = hash(key);
            if( (slots>0) && (0!=__find(key, hkey)) ) return false;
            
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
                HNode *node = __find(key,hkey);
                if(node)
                {
                    assert(hslot[hkey%slots].head==node);
                    KNode *knode = node->knode;
                    destruct( klist.unlink(knode) );
                    kpool.store(knode);
                    hpool.store( hslot[hkey%slots].pop_front() );
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
        void __release() throw()
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
        HNode * __find( param_key key, const size_t hkey ) const throw()
        {
            assert(slots);
            assert(hslot);
            HSlot &slot = hslot[ hkey % slots ];
            for(HNode *node=slot.head;node;node=node->next)
            {
                assert(node->knode);
                if(key == node->knode->key)
                {
                    slot.move_to_front(node);
                    return node;
                }
            }
            return 0;
        }
        
        virtual const_type *lookup( param_key key ) const throw()
        {
            if(slots)
            {
                HNode *node = __find(key,hash(key));
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
    
    
#if 0
	template <
	typename KEY,
	typename T,
	typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global::allocator >
	class map : public associative<KEY,T>
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		
		//! data key/obj node
		class node_type
		{
		public:
			node_type   *next;
			node_type   *prev;
			const size_t hkey;
			const_key    key;
			type         data;
			inline ~node_type() throw() {}
			inline  node_type( size_t h, param_key k,  param_type args ) : next(NULL), prev(NULL), hkey(h), key(k), data(args) {}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
		};
		
		
		
		
		explicit map() throw(): hash_(), hmem_(),ktab_() {}
		explicit map( size_t n, const as_capacity_t & ) : hash_(), hmem_(), ktab_(n,hmem_) {}
		virtual ~map() throw() { _kill(); }
		
		inline map( const map &other ) : hash_(), hmem_(), ktab_()
		{
			map tmp( other.size(), as_capacity );
			other._copy_into( tmp );
			mswap( tmp.ktab_, ktab_ );
		}
		
		//======================================================================
		// container interface
		//======================================================================
		virtual const char *name() const throw()     { return hidden::map_name; }
		virtual size_t      size() const throw()     { return ktab_.nlist.size; }
		virtual size_t      capacity() const throw() { return ktab_.nodes;      }
		virtual void        free() throw()    { _free(); }
		virtual void        release() throw() { _kill(); }
		virtual void        reserve( size_t n )
		{
			if( n > 0 )
			{
				map other( this->capacity() + n, as_capacity );
				_copy_into( other );
				mswap( ktab_, other.ktab_ );
			}
		}
		
		//======================================================================
		// associative interface
		//======================================================================
		virtual bool insert( param_key key, param_type args )
		{
			const size_t     hkey = hash_(key);
			kslot_t         *slot = NULL;
			const node_type *node = ktab_.search( hkey, slot, _match, &key );
			if( node )
			{
				return false;
			}
			else
			{
                __insert(key, args, hkey, slot);
				return true;
			}
		}
		
		virtual bool remove( param_key key ) throw()
		{
			const size_t     hkey = hash_(key);
			kslot_t         *slot = NULL;
			const node_type *node = ktab_.search( hkey, slot, _match, &key );
			if( node )
			{
				assert( slot != NULL); assert( slot->head );
				assert( node == slot->head ->addr); assert( node->key == key );
				ktab_.remove_front_of(slot, destruct<node_type> );
				return true;
			}
			else
				return false;
		}
		
        virtual void lazy_insert( param_key key, param_type args )
        {
            const size_t     hkey = hash_(key);
			kslot_t         *slot = NULL;
			const node_type *node = ktab_.search( hkey, slot, _match, &key );
            if(!node)
                __insert(key, args, hkey, slot);
        }
        
        
		//======================================================================
		// iterators
		//======================================================================
		typedef iterating::linked<type,node_type,iterating::forward> iterator;
		inline iterator begin() throw() { return iterator( ktab_.nlist.head ); }
		inline iterator end()   throw() { return iterator( NULL );       }
		
		typedef iterating::linked<const_type,const node_type,iterating::forward> const_iterator;
		inline const_iterator begin() const throw() { return const_iterator( ktab_.nlist.head  ); }
		inline const_iterator end()   const throw() { return const_iterator( NULL );       }
		
	private:
		YOCTO_DISABLE_ASSIGN(map);
		
		typedef core::ktable<node_type>    ktable_t;
		typedef typename ktable_t::kslot_t kslot_t;
		typedef typename ktable_t::knode_t knode_t;
		mutable KEY_HASHER hash_;
		ALLOCATOR          hmem_;
		mutable ktable_t   ktab_;
		
		virtual const_type *lookup( param_key key ) const throw()
		{
			const size_t     hkey = hash_(key);
			kslot_t         *slot = NULL;
			const node_type *node = ktab_.search( hkey, slot, _match, &key );
			return node ?  & node->data : NULL;
		}
		
		inline void _free() throw() { ktab_.free_with( destruct<node_type> ); }
		inline void _kill() throw() { _free();  ktab_.release_all(hmem_);     }
		inline void _copy_into( map &other ) const
		{
			assert( other.capacity() >= this->size() );
			assert( other.size() == 0 );
			for( const node_type *src = ktab_.nlist.head; src; src = src->next )
			{
				//-- dup/insert
				_insert( other.ktab_, src->hkey, src->key, src->data );
			}
			assert( other.size() == this->size() );
			assert( other.ktab_.signature() == this->ktab_.signature() );
		}
		
		static inline node_type *_create( ktable_t &tab, size_t hkey, param_key key, param_type args )
		{
			//-- atomic node creationg
			node_type       *tgt = tab.cache.query();
			try{ new (tgt) node_type(hkey,key,args); }
			catch(...) { tab.cache.store(tgt); throw; }
			return tgt;
		}
		
		static inline void _insert( ktable_t &tab, size_t hkey, param_key key, param_type args )
		{ tab.insert( _create(tab,hkey,key,args) ); }
		
		static inline void _insert2( ktable_t &tab, size_t hkey, param_key key, param_type args, kslot_t *slot ) throw()
		{ tab.insert2(_create(tab,hkey,key,args),slot); }
		
		
		static inline bool _match( const node_type *node, const void *params ) throw()
		{ return node->key == *(const_key*)params; }
		
        inline void __insert(param_key     key,
                             param_type    args,
                             const size_t  hkey,
                             kslot_t      *slot)
        {
            if( size() < capacity() )
            {
                assert(slot!=NULL);
                _insert2( ktab_, hkey, key, args, slot );
            }
            else
            {
                map  other( container::next_capacity( capacity() ), as_capacity);
                _copy_into(other);
                _insert( other.ktab_,  hkey, key, args );
                mswap( other.ktab_, ktab_ );
            }
        }
	};
#endif
    
}

#endif
