#ifndef YOCTO_GRAPHICS_PIV_ZONE_INCLUDED
#define YOCTO_GRAPHICS_PIV_ZONE_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include "yocto/parallel/split.hpp"

namespace yocto
{
    namespace graphics
    {

        namespace PIV
        {

            class Zone
            {
            public:


                static patch PatchFor( const bitmap &src, const unit_t window_size ) throw()
                {
                    vertex n(3,3);
                    while(n.x*window_size<src.w) ++n.x;
                    n.x -= 2;
                    while(n.y*window_size<src.h) ++n.y;
                    n.y -= 2;
                    return patch( vertex(0,0), n);
                }

                const vertex r;
                const vertex size;
                const vertex dmin;
                const vertex dmax;

                explicit Zone( const bitmap &src, const vertex indx, const vertex count ) throw() :
                r(),
                size(),
                dmin(),
                dmax()
                {
                    assert(indx.x>=0); assert(indx.x<count.x);
                    assert(indx.y>=0); assert(indx.y<count.y);
                    const unit_t W    = src.w;
                    const unit_t H    = src.h;
                    vertex    & _r    = (vertex &)r;
                    vertex    & _size = (vertex &)size;
                    _r.x    = 0;
                    _r.y    = 0;
                    _size.x = W;
                    _size.y = H;
                    parallel::basic_split(indx.x, count.x, _r.x, _size.x);
                    parallel::basic_split(indx.y, count.y, _r.y, _size.y);
                    (unit_t&)(dmin.x) = -r.x;
                    (unit_t&)(dmin.y) = -r.y;
                    (unit_t&)(dmax.x) = W-size.x;
                    (unit_t&)(dmax.y) = H-size.y;
                }

                explicit Zone() throw() :
                r(),
                size(),
                dmin(),
                dmax()
                {
                }
                


                ~Zone() throw() {}


                

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Zone);
            };


            typedef parallel::field2D<Zone> _Zones;

            class Zones : public _Zones
            {
            public:
                typedef parallel::field1D<Zone> Row;

                const unit_t W;
                const unit_t H;

                Zones(const bitmap &src, const unit_t window_size) :
                _Zones( Zone::PatchFor(src,window_size) ),
                W( width.x ),
                H( width.y )
                {
                    _Zones &self = *this;
                    const vertex count(W,H);
                    for(unit_t j=0;j<H;++j)
                    {
                        for(unit_t i=0;i<W;++i)
                        {
                            const vertex indx(i,j);
                            Zone ztmp(src,indx,count);
                            bswap(ztmp,self[j][i]);
                        }
                    }
                }


                virtual ~Zones() throw() {}
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Zones);
            };
        }
    }
}

YOCTO_SUPPORT_C_STYLE_OPS(graphics::PIV::Zone);


#endif

