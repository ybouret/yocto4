#include "yocto/graphics/ops/stencil.hpp"
#include <cstring>

namespace yocto
{

    namespace graphics
    {
        stencil:: row:: row(float *entry) throw() :
        addr(entry+1)
        {
            assert(NULL!=entry);
        }

        float & stencil:: row:: operator[](const unit_t dx) throw()
        {
            assert(dx>=-1);
            assert(dx<=1);
            return addr[dx];
        }

        const float & stencil:: row:: operator[](const unit_t dx) const throw()
        {
            assert(dx>=-1);
            assert(dx<=1);
            return addr[dx];
        }

    }

    namespace graphics
    {
        stencil:: stencil() throw() :
        rows(0),
        v(),
        wksp()
        {
            memset(v,0,sizeof(v));
            setup();
            std::cerr << "sizeof(stencil.wksp)=" << sizeof(wksp) << std::endl;
            std::cerr << "sizeof(stencil.v   )=" << sizeof(v)    << std::endl;
            std::cerr << "sizeof(stencil     )=" << sizeof(stencil) << std::endl;

        }

        void stencil:: setup() throw()
        {
            row *r = (row *)&wksp[0];
            rows = r+1;
            new (&r[0]) row(v+0);
            new (&r[1]) row(v+3);
            new (&r[2]) row(v+6);
        }

        stencil:: ~stencil() throw()
        {
        }


        stencil:: stencil(const stencil &other) throw() :
        rows(0),
        v(),
        wksp()
        {
            memcpy(v,other.v,sizeof(v));
            setup();
        }

        stencil & stencil:: operator=(const stencil &other) throw()
        {
            memmove(v,other.v,sizeof(v));
            return *this;
        }

        stencil::row & stencil:: operator[](const unit_t dy) throw()
        {
            assert(dy>=-1);
            assert(dy<=1);
            return rows[dy];
        }

        const stencil::row & stencil:: operator[](const unit_t dy) const throw()
        {
            assert(dy>=-1);
            assert(dy<=1);
            return rows[dy];
        }


        std::ostream & operator<<( std::ostream &os, const stencil &S )
        {
#if 1
            os << '[';
            os << S[-1][-1] << ' ' << S[-1][0] << ' ' << S[-1][1] << ';';
            os << S[ 0][-1] << ' ' << S[ 0][0] << ' ' << S[ 0][1] << ';';
            os << S[ 1][-1] << ' ' << S[ 1][0] << ' ' << S[ 1][1] << ']';
#endif
            return os;
        }


    }
}