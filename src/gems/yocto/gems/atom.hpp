#ifndef YOCTO_GEMS_ATOM_INCLUDED
#define YOCTO_GEMS_ATOM_INCLUDED 1

#include "yocto/gems/properties.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/math/v3d.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/code/endian.hpp"

namespace yocto
{
    namespace gems
    {
        class library;
        using namespace math;

        //! basic class for library to handle
        class atom_info : public counted_object
        {
        public:
            const word_t      uuid;
            const properties &ppty;

            virtual ~atom_info() throw();


            const word_t & key() const throw();

            typedef intr_ptr<word_t,atom_info> handle;
            typedef set<word_t,handle>         table;

            static bool is_orphan( const handle &p ) throw();
           
        protected:
            explicit atom_info(const word_t u, const properties &p ) throw();
            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(atom_info);
        };


        template <typename T>
        class atom : public atom_info
        {
        public:
            inline virtual ~atom() throw() {}

            const T               mass;
            const T               invm;
            v3d<T>                r;
            v3d<T>                v;
            v3d<T>                a;

            typedef intr_ptr<word_t,atom> pointer;
            typedef set<word_t,pointer>   database;

            static const size_t io_bytes = 2*sizeof(word_t) + 2 * sizeof(T) + 3 * sizeof(v3d<T>);

            inline void append_to( uint8_t * &p ) const throw()
            {
                assert(p);
                store_be(uuid, p);     p += sizeof(uuid);
                store_be(ppty.uuid,p); p += sizeof(uuid);
                store_be(mass,p);      p += sizeof(mass);
                store_be(invm,p);      p += sizeof(invm);
                append_to(p,r);
                append_to(p,v);
                append_to(p,a);
            }

        private:
            friend class library;

            inline atom(const word_t u, const properties &p) throw() :
            atom_info(u,p),
            mass(p.mass),
            invm(T(1)/mass),
            r(),
            v(),
            a()
            {
            }

            static inline void append_to(uint8_t * &p, const v3d<T> &u ) throw()
            {
                store_be(u.x,p); p += sizeof(T);
                store_be(u.y,p); p += sizeof(T);
                store_be(u.z,p); p += sizeof(T);
            }

            
            YOCTO_DISABLE_COPY_AND_ASSIGN(atom);
        };

        template <typename T>
        class binary_atoms
        {
        public:
            explicit binary_atoms() throw() :
            size(0), maxi(0), curr(0), wlen(0), wksp(0) {}

            inline void release() throw()
            {
                memory::kind<memory::global>::release(wksp, wlen);
                maxi=0;
                size=0;
                curr=0;
            }

            virtual ~binary_atoms() throw()
            {
                release();
            }

            inline void build(size_t nmin)
            {
                size=0;
                if(nmin>maxi)
                {
                    memory::kind<memory::global>::release(wksp, wlen);
                    maxi=0;

                    wlen = nmin * atom<T>::io_bytes;
                    wksp = memory::kind<memory::global>::acquire(wlen);
                    maxi = wlen/atom<T>::io_bytes;
                }
                curr = (uint8_t *)wksp;
            }

            inline void reset() throw() { size=0; curr = (uint8_t *)wksp; }
            inline void store( const atom<T> &a ) throw()
            {
                assert(size<maxi);
                a.append_to(curr);
                ++size;
            }

            

        private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(binary_atoms);
            size_t   size;
            size_t   maxi;
            uint8_t *curr;
            size_t   wlen;
            void    *wksp;
        };
    }
}

#endif
