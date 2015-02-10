#ifndef YOCTO_GEMS_ATOMS_INCLUDED
#define YOCTO_GEMS_ATOMS_INCLUDED 1

#include "yocto/gems/atom.hpp"
#include "yocto/sequence/addr-list.hpp"

namespace yocto
{
    namespace gems
    {

        //! a node for one atom
        template <typename T>
        class atom_node
        {
        public:
            typedef typename atom<T>::pointer atom_ptr;

            inline atom_node( const atom_ptr &p) throw() :
            a(p),
            next(0),
            prev(0)
            {
            }

            inline atom_node( const atom<T> *p ) throw() :
            a(p),
            next(0),
            prev(0)
            {
            }

            inline ~atom_node() throw() {}

            atom_ptr   a;
            atom_node *next;
            atom_node *prev;

            YOCTO_MAKE_OBJECT;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(atom_node);
        };

        template <typename T>
        class atoms
        {
        public:
            typedef atom_node<T>                 node_type;
            typedef core::list_of_cpp<node_type> list_type;
            typedef addr_node<node_type>         hook_type;
            typedef core::list_of_cpp<hook_type> slot_type;

            explicit atoms() : atm(), nslot(0), slots(0) {}
            virtual ~atoms() throw() { release(); }

            inline void free() throw()
            {
                atm.clear();
                for(size_t i=0;i<nslot;++i)
                {
                    slots[i].clear();
                }
            }

            inline void release() throw()
            {
                free();
                memory::kind<memory::global>::release_as(slots,nslot);
            }

        private:
            list_type  atm;
            size_t     nslot;
            slot_type *slots;

            YOCTO_DISABLE_COPY_AND_ASSIGN(atoms);
        };
        
        
    }
}

#endif
