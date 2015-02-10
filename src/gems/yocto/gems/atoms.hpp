#ifndef YOCTO_GEMS_ATOMS_INCLUDED
#define YOCTO_GEMS_ATOMS_INCLUDED 1

#include "yocto/gems/atom.hpp"
#include "yocto/sequence/addr-list.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

#include <iostream>

namespace yocto
{
    namespace gems
    {

        //______________________________________________________________________
        //
        //! a node for one atom
        //______________________________________________________________________
        template <typename T>
        class atom_node
        {
        public:
            typedef typename atom<T>::pointer atom_ptr;

            inline atom_node( const atom_ptr &p) throw() :
            pAtom(p),
            next(0),
            prev(0)
            {
            }

            inline atom_node( const atom<T> *p ) throw() :
            pAtom(p),
            next(0),
            prev(0)
            {
            }

            inline ~atom_node() throw() {}

            atom_ptr   pAtom;
            atom_node *next;
            atom_node *prev;

            YOCTO_MAKE_OBJECT;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(atom_node);
        };

        //______________________________________________________________________
        //
        //! a dynamic set of atoms
        //______________________________________________________________________
        template <typename T>
        class atoms
        {
        public:
            typedef typename atom<T>::pointer    atom_ptr;
            typedef atom_node<T>                 node_type;
            typedef core::list_of_cpp<node_type> list_type;
            typedef addr_node<node_type>         hook_type;
            typedef core::list_of_cpp<hook_type> slot_type;

            //! default ctor
            explicit atoms() : alist(), nslot(0), smask(0), slots(0)
            {
                std::cerr << "sizeof(node_type)=" << sizeof(node_type) << std::endl;
                std::cerr << "sizeof(hook_type)=" << sizeof(hook_type) << std::endl;
            }

            //! default dtor
            virtual ~atoms() throw() { release(); }

            //! free atoms, keep slots and hooks
            inline void free() throw()
            {
                alist.clear();
                for(size_t i=0;i<nslot;++i)
                {
                    slots[i].clear();
                }
            }

            //! release all resources
            inline void release() throw()
            {
                free();
                memory::kind<memory::global>::release_as(slots,nslot);
                smask = 0;
            }

            //! check memory for load factor
            inline void expand_slots()
            {

                const size_t nmin = max_of<size_t>(1,(alist.size+1)/4);

                //______________________________________________________________
                //
                // allocate new global memory
                //______________________________________________________________
                size_t new_nslot = max_of<size_t>(8,next_power_of_two(nmin));
                if(new_nslot<=nslot)
                {
                    return;
                }

                assert(is_a_power_of_two(new_nslot));
                std::cerr << "-- expand " << nslot << " ==> " << new_nslot << std::endl;
                slot_type   *new_slots = memory::kind<memory::global>::acquire_as<slot_type>(new_nslot);
                const size_t new_smask = new_nslot - 1;

                //______________________________________________________________
                //
                //transfert hooks
                //______________________________________________________________
                for(size_t i=0;i<nslot;++i)
                {
                    slot_type &slot = slots[i];
                    while(slot.size)
                    {
                        hook_type *hook = slot.pop_back();
                        new_slots[ hook->addr->pAtom->uuid & new_smask ].push_back(hook);
                    }
                }

                //______________________________________________________________
                //
                //update memory
                //______________________________________________________________
                memory::kind<memory::global>::release_as(slots,nslot);
                slots = new_slots;
                nslot = new_nslot;
                smask = new_smask;
            }

            //! insert an atom ptr
            inline void insert( const atom_ptr &ptr )
            {
                expand_slots();
                slot_type *slot = 0;
                node_type *node = lookup_node(ptr->uuid, &slot);
                if(node)
                {
                    throw exception("multiple atom %u",unsigned(ptr->uuid));
                }

                //______________________________________________________________
                //
                // create node for list
                //______________________________________________________________
                node = new node_type(ptr);


                //______________________________________________________________
                //
                // create hook for slots: node is taken care of in case of failure
                //______________________________________________________________
                hook_type *hook = create_hook(node);

                //______________________________________________________________
                //
                // dual insertion
                //______________________________________________________________
                slot->push_front(hook);
                alist.push_back(node);

            }

            //! helper
            inline void insert( atom<T> *a )
            {
                const atom_ptr ptr(a);
                insert(ptr);
            }

            inline void transfer_to( atoms &other, const word_t uuid )
            {
                if(nslot>0)
                {
                    slot_type &slot = slots[ uuid & smask ];
                    for(hook_type *hook = slot.head;hook;hook=hook->next)
                    {
                        node_type *node = hook->addr;
                        if( uuid == node->pAtom->uuid)
                        {
                            other.expand_slots();
                            other.slots[ uuid & other.smask ].push_front( slot.unlink(hook) );
                            other.alist.push_back( alist.unlink(node) );
                            return;
                        }
                    }
                }
                throw exception("atoms.tranfer: no uuid %u", unsigned(uuid) );
            }

            inline size_t size() const throw() { return alist.size; }


        private:
            list_type          alist;
            size_t             nslot; //!< a power of two
            size_t             smask; //!< nslot-1
            mutable slot_type *slots;

            //! use the pool
            inline
            hook_type *create_hook(node_type *node)
            {
                assert(node);

                auto_ptr<node_type> guard(node);
                hook_type *hook = new hook_type(node);
                guard.forget();
                return hook;

            }


            inline
            node_type *lookup_node(const word_t uuid,
                                   slot_type  **ppSlot) const throw()
            {
                assert(ppSlot);
                assert(0== *ppSlot);

                if(nslot>0)
                {
                    assert(nslot>0);
                    assert(nslot-1==smask);
                    assert(slots!=0);

                    slot_type *slot = (slot_type *) & slots[ uuid & smask ];
                    *ppSlot =  slot;
                    for(hook_type *hook = slot->head;hook;hook=hook->next)
                    {
                        node_type *node = hook->addr;
                        if(uuid == node->pAtom->uuid)
                        {
                            slot->move_to_front(hook);
                            return node;
                        }
                    }
                }
                return 0;
            }
            
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(atoms);
        };
        
        
    }
}

#endif
