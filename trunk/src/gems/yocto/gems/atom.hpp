#ifndef YOCTO_GEMS_ATOM_INCLUDED
#define YOCTO_GEMS_ATOM_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/ptr/intr.hpp"

#include "yocto/math/v3d.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/container/vslot.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace gems
    {
        using namespace math;
        
        template <typename T>
        class properties : public counted_object
        {
        public:
            typedef intr_ptr<string,properties> pointer;
            typedef set<string,pointer>         db;
            
            const string name;
            const word_t type;
            const T      mass;
            vslot        data;
            
            explicit properties(const string &n, const word_t t, const T m) :
            name(n),
            type(t),
            mass(m),
            data()
            {
            }
            
            virtual ~properties() throw()
            {
            }
            
            inline const string & key() const throw() { return name; }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };
        
        template <typename T>
        class library : public properties<T>::db
        {
        public:
            typedef typename properties<T>::pointer ppty_t;
            explicit library() :  properties<T>::db(8,as_capacity) {}
            virtual ~library() throw() {}
            
            inline void append( const string &name, const word_t type, const T mass )
            {
                ppty_t p( new properties<T>(name,type,mass) );
                if(!this->insert(p))
                    throw exception("multiple properties '%s'", name.c_str());
            }
            
            inline const ppty_t & operator[](const string &name) const
            {
                const ppty_t *pp = this->search(name);
                if(!pp) throw exception("no properties '%s'", name.c_str());
                return *pp;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
        
        template <typename T>
        class atom : public counted_object
        {
        public:
            typedef arc_ptr<atom>                   pointer;
            typedef vector<pointer>                 db;
            typedef typename properties<T>::pointer ppty_t;

            const word_t uuid; //!< unique ID
            T            m;    //!< mass
            T            w;    //!< 1/mass
            v3d<T>       r;    //!< position
            v3d<T>       v;    //!< velocity
            v3d<T>       f;    //!< force accumulator
            ppty_t       ppty; //!< for force fields
            
            inline atom(word_t u,const ppty_t &p) throw() :
            uuid(u),
            m(1),
            w(1),
            r(),
            v(),
            f(),
            ppty(p)
            {
                set_mass(ppty->mass);
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
            typedef typename properties<T>::pointer ppty_t;
            explicit atoms() throw() : atom<T>::db() {}
            virtual ~atoms() throw() {}
            explicit atoms(size_t n) : atom<T>::db(n,as_capacity) {}
            
            void append(word_t u,const ppty_t &p)
            {
                typename atom<T>::pointer pa( new atom<T>(u,p) );
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(atoms);
        };
        
    }
}

#endif