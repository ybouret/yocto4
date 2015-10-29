#ifndef YOCTO_GRAPHIX_PIXMAP_INCLUDED
#define YOCTO_GRAPHIX_PIXMAP_INCLUDED 1

#include "yocto/graphix/bitmap.hpp"
#include "yocto/container/cslot.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace graphix
    {

        //______________________________________________________________________
        //
        // a structure of pixels over bitmaps
        //______________________________________________________________________
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            class row
            {
            public:
                inline row(size_t W,void *P) throw() : w(W), p(static_cast<T*>(P)) {}

                inline T &operator[](unit_t i) throw()
                { assert(i>=0); assert(i<w); return p[i]; }

                inline const T &operator[](unit_t i) const throw()
                { assert(i>=0); assert(i<w); return p[i]; }

                const size_t w;

            private:
                T          *p;
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                ~row() throw();
            };


            virtual ~pixmap() throw()
            {
            }


            inline pixmap(size_t W,size_t H) :
            bitmap( sizeof(T),W,H ),
            rmem( h*sizeof(row) ),
            rows( static_cast<row*>(rmem.data) )
            {
                setup_rows();
            }

            inline row &operator[](const unit_t j) throw()
            {
                assert(j>=0);
                assert(j<h);
                return rows[j];
            }

            inline const row &operator[](const unit_t j) const throw()
            {
                assert(j>=0);
                assert(j<h);
                return rows[j];
            }


            inline pixmap( const pixmap &other ) :
            bitmap(other),
            rmem( h*sizeof(row) ),
            rows( static_cast<row*>(rmem.data) )
            {
                setup_rows();
            }

            inline pixmap(const pixmap &other, const patch &area) :
            bitmap(other,area),
            rmem( h*sizeof(row) ),
            rows( static_cast<row*>(rmem.data) )
            {
                setup_rows();
            }

            inline pixmap(bitmap *bmp) :
            bitmap(bmp),
            rmem( h*sizeof(row) ),
            rows( static_cast<row*>(rmem.data) )
            {
                if(sizeof(T)!=depth)
                    throw exception("pixmap: invalid depth=%ld from bitmap, expecting %u!", long(depth), unsigned(sizeof(T)));
                setup_rows();
            }


        private:
            YOCTO_DISABLE_ASSIGN(pixmap);
            cslot  rmem;
            row   *rows;

            inline void setup_rows() throw()
            {
                for(unit_t j=0;j<h;++j)
                {
                    new (rows+j) row(w,get_line(j));
                }
            }
        };

    }
}

#endif

