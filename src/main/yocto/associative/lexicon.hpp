#ifndef YOCTO_LEXICON_INCLUDED
#define YOCTO_LEXICON_INCLUDED 1

#include "yocto/type/key.hpp"
#include "yocto/associative/key-hasher.hpp"

#include "yocto/container/container.hpp"
#include "yocto/container/iter-handle.hpp"

#include "yocto/core/list.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/code/htable.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sequence/lw-array.hpp"

#include <iostream>

namespace yocto
{
    
    
    namespace hidden
    {
        extern const char lexicon_name[];
    }
    
    
    //! compact set
    /**
     Each object must have a 'const_key &key() const throw()' function.
     The key part of each object MUST be constant...
     The keys MUST have an operator== and and operator<.
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
        
        class Node
        {
        public:
            Node        *next;
            Node        *prev;
            const size_t hkey;
            type         data;
            inline  Node(const size_t k, param_type &args) :
            next(0),
            prev(0),
            hkey(k),
            data(args) {}
            
            inline ~Node() throw() {}
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Node);
        };
        
        typedef core::list_of<Node>   Slot;
        typedef memory::slab_of<Node> Pool;
        typedef mutable_type         *Hook;
        
        inline virtual const char *name() const throw() { return hidden::lexicon_name; }
        inline virtual size_t size()      const throw() { return items; }
        inline virtual size_t capacity()  const throw() { return itmax; }
        inline         size_t num_lists() const throw() { return slots; }
        inline virtual void   reserve(size_t n) { if(n) grow(n); }
        inline virtual void   free() throw() { __free(); }
        inline virtual void   release() throw() { __release(); }
        inline         size_t bytes() const throw() { return wlen; }
        
        inline explicit lexicon() throw() :
        items(0),
        itmax(0),
        slot(0),
        slots(0),
        pool(0,0),
        hook(0),
        hvec(0),
        hash(),
        hmem(),
        wlen(0),
        wksp(0)
        {
        }
        
        inline explicit lexicon(size_t n) :
        items(0),
        itmax(0),
        slot(0),
        slots(0),
        pool(0,0),
        hook(0),
        hvec(0),
        hash(),
        hmem(),
        wlen(0),
        wksp(0)
        {
            build(n);
        }
        
        
        
        
        inline virtual ~lexicon() throw()
        {
            __release();
        }
        
        //! no-throw swap
        inline void swap_with( lexicon &other ) throw()
        {
            cswap(items, other.items);
            cswap(itmax, other.itmax);
            cswap(slot,  other.slot);
            cswap(slots, other.slots);
            pool.swap_with(other.pool);
            cswap(hook,other.hook);
            cswap(hvec,other.hvec);
            cswap(wlen,other.wlen);
            cswap(wksp,other.wksp);
        }
        
        //! no throw search
        inline type * search( param_key key ) throw()
        {
            return find(key);
        }
        
        //! no throw search
        inline const_type * search( param_key key ) const throw()
        {
            return find(key);
        }
        
        //! insert a NEW key/hkey, assuming enough room
        inline void __insert( const size_t hkey, param_type args )
        {
            assert(items<itmax);
            assert(pool.available() == itmax - items);
            assert(!find( args.key() ));
            assert(slot);
            assert(slots);
            register_hook( append_to( &slot[hkey%slots],hkey,args) );
            assert(items<=itmax);
        }
        
        //! try to insert a new object
        inline bool insert( param_type args )
        {
            const_key   &key  = args.key();
            const size_t hkey = hash(key);
            if(slots)
            {
                //______________________________________________________________
                //
                // look up
                //______________________________________________________________
                Slot *s = &slot[ hkey % slots ];
                for(const Node *node = s->head;node;node=node->next)
                {
                    if(node->data.key() == key )
                    {
                        return false;
                    }
                }
                
                //______________________________________________________________
                //
                // key was not found
                //______________________________________________________________
                if(items>=itmax)
                {
                    lexicon tmp( this->next_capacity(itmax) );
                    duplicate_into(tmp);
                    tmp.__insert(hkey,args);
                    swap_with(tmp);
                }
                else
                {
                    assert(pool.available()>0);
                    register_hook( append_to(s, hkey, args) );
                }
                return true;
            }
            else
            {
                assert(0==itmax);
                assert(0==items);
                build(1); // minimal lexicon
                __insert(hkey,args);
                return true;
            }
        }
        
        inline bool remove( param_key key ) throw()
        {
            if(slots)
            {
                const size_t hkey = hash(key);
                Slot *s = &slot[hkey%slots];
                for(Node *node = s->head;node;node=node->next)
                {
                    if(key == node->data.key() )
                    {
                        __remove( s->unlink(node) );
                        return true;
                    }
                }
            }
            return false;
        }
        
        
        //! forward iterator
        typedef iterating::handle<type,iterating::forward> iterator;
        inline iterator begin() throw() { return iterator(hook);       }
        inline iterator end()   throw() { return iterator(hook+items); }
        
        //! forward const iterator
        typedef iterating::handle<const_type,iterating::forward> const_iterator;
        inline const_iterator begin() const throw() { return const_iterator(hook);       }
        inline const_iterator end()   const throw() { return const_iterator(hook+items); }
        
        //! fast access
        inline type & operator[](size_t i) throw()
        {
            assert(i>=1);
            assert(i<=items);
            assert(hvec);
            assert(hvec[i]);
            return *hvec[i];
        }
        
        //! fast access
        inline const_type & operator[](size_t i) const throw()
        {
            assert(i>=1);
            assert(i<=items);
            assert(hvec);
            assert(hvec[i]);
            return *hvec[i];
        }
        
        
    private:
        size_t items;  //!< current size
        size_t itmax;  //!< maximum capacity
        Slot  *slot;   //!< where slots are
        size_t slots;  //!< #slots
        Pool   pool;   //!< pool of embedded nodes
        Hook  *hook;   //!< array of hook
        Hook  *hvec;   //!< hook-1, for access
        
        mutable KEY_HASHER hash;
        ALLOCATOR          hmem;
        
        size_t wlen;  //!< total allocated bytes
        void  *wksp;  //!< where memory is
        YOCTO_DISABLE_COPY_AND_ASSIGN(lexicon);
        
        //______________________________________________________________________
        //
        // allocate and prepare memory
        //______________________________________________________________________
        inline void build(size_t n)
        {
            assert(0==items);
            if(n>0)
            {
                //______________________________________________________________
                //
                // compute memory needs
                //______________________________________________________________
                size_t       num_items   = n;
                const size_t num_slots   = htable::compute_slots_for(num_items);
                const size_t slot_offset = 0;
                const size_t slot_length = num_slots * sizeof(Slot);
                
                const size_t pool_offset = memory::align(slot_offset+slot_length);
                const size_t pool_length = Pool::bytes_for(num_items);
                
                const size_t hook_offset = memory::align(pool_offset+pool_length);
                const size_t hook_length = num_items * sizeof(Hook);
                
                //______________________________________________________________
                //
                // allocate
                //______________________________________________________________
                wlen = memory::align(hook_offset+hook_length);
                wksp = hmem.acquire(wlen);
                
                //______________________________________________________________
                //
                //dispatch
                //______________________________________________________________
                uint8_t *p = (uint8_t *)wksp;
                slot       = (Slot *) &p[slot_offset];
                pool.         format( &p[pool_offset], num_items);
                hook       = (Hook *) &p[hook_offset];
                hvec       = hook-1;
                
                //______________________________________________________________
                //
                // finish
                //______________________________________________________________
                itmax = num_items;
                slots = num_slots;
                
            }
        }
        
        //______________________________________________________________________
        //
        // free content, keep memory
        //______________________________________________________________________
        inline void __free() throw()
        {
            // destruct nodes+data
            for(size_t i=0;i<slots;++i)
            {
                Slot &s = slot[i];
                while(s.size)
                {
                    Node *node = s.pop_back();
                    node->~Node();
                    pool.store(node);
                }
            }
            // clear hooks
            for(size_t i=0;i<items;++i)
            {
                hook[i] = 0;
            }
            items = 0;
        }
        
        //______________________________________________________________________
        //
        // free content and memory
        //______________________________________________________________________
        inline void __release() throw()
        {
            __free();
            pool.format(0,0);
            itmax = 0;
            slots = 0;
            slot  = 0;
            hook  = 0;
            hmem.release(wksp,wlen);
        }
        
        //______________________________________________________________________
        //
        // find an item
        //______________________________________________________________________
        inline type * find( param_key key ) const throw()
        {
            if(slots)
            {
                assert(slot);
                Slot &s = slot[ hash(key) % slots ];
                for(Node *node = s.head;node;node=node->next)
                {
                    if( node->data.key() == key )
                    {
                        return & (node->data);
                    }
                }
            }
            return 0;
        }
        
        //______________________________________________________________________
        //
        // grow
        //______________________________________________________________________
        inline void grow(size_t n)
        {
            assert(n>0);
            lexicon tmp( itmax+n );
            duplicate_into(tmp);
            swap_with(tmp);
        }
        
        
        static int compare_hook_by_keys( const Hook &lhs, const Hook &rhs ) throw()
        {
            const_key &L = lhs->key();
            const_key &R = rhs->key();
            return (L < R) ? -1 : ( R < L ? 1 : 0 );
        }
        
        //______________________________________________________________________
        //
        // fast duplication
        //______________________________________________________________________
        inline void duplicate_into( lexicon &target ) const
        {
            assert(0==target.size());
            assert(target.capacity()>=this->size());
            
            //__________________________________________________________________
            //
            // for all slots
            //__________________________________________________________________
            for(size_t i=0;i<slots;++i)
            {
                //______________________________________________________________
                //
                // for all nodes
                //______________________________________________________________
                assert(slot);
                for( const Node *node = slot[i].head;node;node=node->next)
                {
                    const size_t hkey = node->hkey;
                    Slot        *stgt = target.slot + (hkey%target.slots);
                    target.hook[target.items] = target.append_to( stgt , hkey, node->data);
                    ++target.items;
                    // the target hooks are not sorted
                }
            }
            assert(items==target.items);
            lw_array<Hook> hooks(target.hook,target.items);
            quicksort(hooks,compare_hook_by_keys);
        }
        
        //______________________________________________________________________
        //
        // register a hook and increase #items
        //______________________________________________________________________
        inline void register_hook( type *addr ) throw()
        {
            assert(hook);
            assert(items<itmax);
            assert(addr!=0);
            
            //append addr at the end of hooks
            size_t j = items++;
            hook[j]  = addr;
            while(j>0)
            {
                Hook &curr = hook[j];
                Hook &prev = hook[--j];
                if( curr->key()<prev->key() )
                {
                    cswap(curr,prev);
                }
                else
                {
                    return;
                }
            }
        }
        
        //______________________________________________________________________
        //
        // make data and insert its node
        //______________________________________________________________________
        inline type *append_to( Slot *s, const size_t hkey, param_type args)
        {
            assert(s);
            assert(slot+(hkey%slots)==s);
            assert(pool.available()>0);
            Node *node = pool.query();
            try
            {
                s->push_front( new (node) Node(hkey,args) );
            }
            catch(...)
            {
                pool.store(node);
                throw;
            }
            return & (node->data);
        }
        
        //______________________________________________________________________
        //
        // make data and insert its node
        //______________________________________________________________________
        inline void __remove(Node *node) throw()
        {
            assert(items>0);
            const_key &key = node->data.key();
            
            //__________________________________________________________________
            //
            // locate the handle
            //__________________________________________________________________
            size_t lo = 0;
            size_t hi = items-1;
            while(true)
            {
                const size_t mid = lo+((hi-lo)>>1);
                const Hook   h   = hook[mid];
                if(h->key() == key )
                {
                    //__________________________________________________________
                    //
                    // remove node/data
                    //__________________________________________________________
                    node->~Node();
                    pool.store(node);
                    
                    //__________________________________________________________
                    //
                    // move data
                    //__________________________________________________________
                    --items;
                    for(size_t i=mid;i<items;++i)
                    {
                        bmove(hook[i], hook[i+1]);
                    }
                    return;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // bisection part
                    //__________________________________________________________
                    if(h->key()<key )
                    {
                        lo = mid+1;
                    }
                    else
                    {
                        hi = mid-1;
                    }
                }
            }
            
        }
    };
    
    
}

#endif
