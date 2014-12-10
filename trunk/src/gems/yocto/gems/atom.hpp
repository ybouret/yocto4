#ifndef YOCTO_GEMS_ATOM_INCLUDED
#define YOCTO_GEMS_ATOM_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/math/v3d.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace gems
    {
        using namespace math;
        
        template <typename T>
        class atom : public counted_object
        {
        public:
            typedef arc_ptr<atom>    pointer;
            typedef vector<pointer>  db;
            
            const word_t uuid; //!< unique ID
            T            m;    //!< mass
            T            w;    //!< 1/mass
            v3d<T>       r;    //!< position
            v3d<T>       v;    //!< velocity
            v3d<T>       f;    //!< force accumulator
            
            inline atom(word_t u,T mass) throw() :
            uuid(u),
            m(1),
            w(1),
            r(),
            v(),
            f()
            {
                set_mass(m);
            }
            
            inline void set_mass(T mass) throw()
            {
                m = mass;
                w = 1/m;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(atom);
        };
        
        template <typename T>
        class atoms : public atom<T>::db
        {
        public:
            explicit atoms() throw() : atom<T>::db() {}
            virtual ~atoms() throw() {}
            explicit atoms(size_t n) : atom<T>::db(n,as_capacity) {}
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(atoms);
        };
        
    }
}

#endif