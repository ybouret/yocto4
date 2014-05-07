#ifndef YOCTO_LEXICON_INCLUDED
#define YOCTO_LEXICON_INCLUDED 1

#include "yocto/type/key.hpp"
#include "yocto/associative/key-hasher.hpp"

#include "yocto/container/container.hpp"
#include "yocto/container/iter-handle.hpp"

#include "yocto/core/list.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/code/htable.hpp"

#include <iostream>

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
        
        //! insert a new key/hkey, assuming enough room
        inline void __insert( const size_t hkey, param_type args )
        {
            assert(items<itmax);
            assert(!find( args.key() ));
            assert(slot);
            assert(slots);
            register_hook( append_to( &slot[hkey%slots],hkey,args) );
        }
        
        //! try to insert a new object
        inline bool insert( param_type args )
        {
            const_key   &key  = args.key();
            const size_t hkey = hash(key);
            if(slots)
            {
                // look up
                Slot *s = &slot[ hkey % slots ];
                for(const Node *node = s->head;node;node=node->next)
                {
                    if(node->data.key() == key )
                    {
                        return false;
                    }
                }
                
                // key was not found
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
                assert(itmax==0);
                build(1); // minimal lexicon
                __insert(hkey,args);
                return true;
            }
        }
        
        
        //! forward iterator
        typedef iterating::handle<type,iterating::forward> iterator;
        inline iterator begin() throw() { return iterator(hook);       }
        inline iterator end()   throw() { return iterator(hook+items); }
            
        
    private:
        size_t items;  //!< current size
        size_t itmax;  //!< maximum capacity
        Slot  *slot;   //!< where slots are
        size_t slots;  //!< #slots
        Pool   pool;   //!< pool of embedded nodes
        Hook  *hook;   //!< array of hook
        
        
        mutable KEY_HASHER hash;
        ALLOCATOR          hmem;
        
        size_t wlen;  //!< total allocated bytes
        void  *wksp;  //!< where memory is
        YOCTO_DISABLE_COPY_AND_ASSIGN(lexicon);
        
        //______________________________________________________________________
        //
        // allocate and prepare memory
        //______________________________________________________________________
        void build(size_t n)
        {
            assert(0==items);
            if(n>0)
            {
                // compute memory needs
                size_t       num_items   = n;
                const size_t num_slots   = htable::compute_slots_for(num_items);
                const size_t slot_offset = 0;
                const size_t slot_length = num_slots * sizeof(Slot);
                
                const size_t pool_offset = memory::align(slot_offset+slot_length);
                const size_t pool_length = Pool::bytes_for(num_items);
                
                const size_t hook_offset = memory::align(pool_offset+pool_length);
                const size_t hook_length = num_items * sizeof(Hook);
                
                // allocate
                wlen = memory::align(hook_offset+hook_length);
                wksp = hmem.acquire(wlen);
                
                //dispatch
                uint8_t *p = (uint8_t *)wksp;
                slot       = (Slot *) &p[slot_offset];
                pool.         format( &p[pool_offset], num_items);
                hook       = (Hook *) &p[hook_offset];
                
                // finish
                itmax = num_items;
                slots = num_slots;
                
                assert(pool.available()==itmax);
            }
        }
        
        //______________________________________________________________________
        //
        // free content, keep memory
        //______________________________________________________________________
        inline void __free() throw()
        {
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
            
            for(size_t i=0;i<itmax;++i)
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
                        //s.move_to_front(node);
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
        
        //______________________________________________________________________
        //
        // fast duplication
        //______________________________________________________________________
        inline void duplicate_into( lexicon &lx ) const
        {
            std::cerr << "dup.init" << std::endl;
            assert(0==lx.size());
            assert(lx.capacity()>=this->size());
            
            // for all slots
            size_t j = 0;
            for(size_t i=0;i<slots;++i)
            {
                // for all node
                assert(slot);
                for( const Node *node = slot[i].head;node;node=node->next)
                {
                    const size_t hkey = node->hkey;
                    lx.hook[j] = lx.append_to( lx.slot + (hkey%lx.slots), hkey, node->data);
                    ++j;
                    // the hooks are not sorted
                }
            }
            assert(items==j);
            std::cerr << "dup.done" << std::endl;

        }
        
        //______________________________________________________________________
        //
        // append a hook
        //______________________________________________________________________
        inline void register_hook( type *addr ) throw()
        {
            assert(hook);
            assert(items<itmax);
            assert(addr!=0);
        
            //append addr at the end of hooks
            size_t i = items;
            hook[i]  = addr;
            
            ++items;
        }
        
        //______________________________________________________________________
        //
        //
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
            return & node->data;
        }
        
    };
    
    
}

#endif
