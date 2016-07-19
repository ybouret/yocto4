#ifndef YOCTO_GFX_OPS_STENCIL_INCLUDED
#define YOCTO_GFX_OPS_STENCIL_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/bzset.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {

        class _stencil
        {
        public:
            static const vertex shift[9];

            inline virtual ~ _stencil() throw()
            {
            }

        protected:
            inline _stencil() throw() {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(_stencil);
        };

        template <typename T>
        class stencil
        {
        public:
            class row
            {
            public:
                inline row(T *entry)  throw() : addr(entry+1) { assert(entry); }

                inline T &      operator[](const unit_t dx) throw()
                {
                    assert(dx>=-1); assert(dx<=1);return addr[dx];
                }

                const T &operator[](const unit_t dx) const throw()
                {
                    assert(dx>=-1); assert(dx<=1);return addr[dx];
                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                ~row() throw();
                T *addr;
            };

            inline stencil() throw() : rows(0), v(), wksp()
            {
                ldz();
                setup();
            }

            inline virtual ~stencil() throw() {}

            inline stencil(const stencil &other) throw() :
            rows(0),
            v(),
            wksp()
            {
                memcpy(v,other.v,sizeof(v));
                setup();
            }

            inline stencil & operator=(const stencil &other) throw()
            {
                memmove(v,other.v,sizeof(v));
                return *this;
            }

            inline row & operator[](const unit_t dy) throw()
            {
                assert(dy>=-1);assert(dy<=1); return rows[dy];
            }

            inline const row & operator[](const unit_t dy) const throw()
            {
                assert(dy>=-1);assert(dy<=1); return rows[dy];
            }



            inline friend  std::ostream & operator<<( std::ostream &os, const stencil &S )
            {
                os << '[';
                os << S[-1][-1] << ' ' << S[-1][0] << ' ' << S[-1][1] << ';';
                os << S[ 0][-1] << ' ' << S[ 0][0] << ' ' << S[ 0][1] << ';';
                os << S[ 1][-1] << ' ' << S[ 1][0] << ' ' << S[ 1][1] << ']';
                return os;
            }

            inline void ldz() throw()
            {
                memset(v,0,sizeof(v));
            }


            template <typename U>
            inline void load(const pixmap<U> &src,
                             const vertex     org) throw()
            {
                assert(src.has(org));
                for(size_t i=0;i<9;++i)
                {
                    const vertex probe = org+ _stencil::shift[i];
                    if(src.has(probe))
                    {
                        v[i] = T(src[probe]);
                    }
                    else
                    {
                        bzset(v[i]);
                    }
                }

            }

            inline T       & operator()(const size_t indx) throw()       { assert(indx<9); return v[indx]; }
            inline const T & operator()(const size_t indx) const throw() { assert(indx<9); return v[indx]; }

        private:
            row     *rows;
            T        v[9];
            uint64_t wksp[ YOCTO_U64_FOR_SIZE( 3*sizeof(row) ) ];
            inline void setup() throw()
            {
                row *r = (row *)&wksp[0];
                rows = r+1;
                new (&r[0]) row(v+0);
                new (&r[1]) row(v+3);
                new (&r[2]) row(v+6);
            }
            
        };

    }
}

#endif
