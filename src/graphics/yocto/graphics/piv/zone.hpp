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

            class Zone : public patch
            {
            public:


                static patch MetaPatchFor( const bitmap &src, const unit_t window_size ) throw()
                {
                    vertex n(3,3);
                    while(n.x*window_size<src.w) ++n.x;
                    n.x -= 2;
                    while(n.y*window_size<src.h) ++n.y;
                    n.y -= 2;
                    return patch( vertex(0,0), n);
                }

                static patch SubPatch(const bitmap &src, const vertex indx, const vertex count) throw()
                {

                    assert(indx.x>=0); assert(indx.x<count.x);
                    assert(indx.y>=0); assert(indx.y<count.y);
                    unit_t X = 0;
                    unit_t Y = 0;
                    unit_t W = src.w;
                    unit_t H = src.h;
                    parallel::basic_split(indx.x, count.x,X,W);
                    parallel::basic_split(indx.y, count.y,Y,H);
                    const vertex lo(X,Y);
                    const vertex hi(X+W-1,Y+H-1);
                    return patch(lo,hi);
                }

                const vertex dmin;
                const vertex dmax;

                explicit Zone( const bitmap &src, const vertex indx, const vertex count ) throw() :
                patch( SubPatch(src,indx,count) ),
                dmin(),
                dmax(),
                data(0),
                dmem(0)
                {
                    (unit_t&)(dmin.x) = -lower.x;
                    (unit_t&)(dmin.y) = -lower.y;
                    (unit_t&)(dmax.x) = src.w-width.x;
                    (unit_t&)(dmax.y) = src.h-width.y;
                }

                explicit Zone() throw() :
                patch( vertex(), vertex() ),
                dmin(),
                dmax()
                {
                }
                


                ~Zone() throw()
                {
                    memory::kind<memory::global>::release_as(data,dmem);
                }


                float *data;
                size_t dmem;

                void allocate()
                {
                    assert(dmem<=0);
                    assert(0==data);
                    dmem = items;
                    data = memory::kind<memory::global>::acquire_as<float>(dmem);
                }

                void load( const pixmapf &src ) throw()
                {
                    assert(data);
                    assert(dmem>=items);
                    assert(src.contains(*this));
                    size_t k = 0;
                    for(unit_t y=upper.y;y>=lower.y;--y)
                    {
                        const pixmapf::row &src_y = src[y];
                        for(unit_t x=upper.x;x>=lower.x;--x)
                        {
                            data[k++] = src_y[x];
                        }
                    }
                }

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
                _Zones( Zone::MetaPatchFor(src,window_size) ),
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
                            self[j][i].allocate();
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



#endif

