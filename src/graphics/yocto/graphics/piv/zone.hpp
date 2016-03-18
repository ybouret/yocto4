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


                //! get meta patch numbers for a given window size
                static patch MetaPatchFor( const bitmap &src, const unit_t window_size ) throw()
                {
                    vertex n(3,3);
                    while(n.x*window_size<src.w) ++n.x;
                    n.x -= 2;
                    while(n.y*window_size<src.h) ++n.y;
                    n.y -= 2;
                    return patch( vertex(0,0), n);
                }

                //! split according to 2D MPI-like pattern
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
                original(0),
                org_diff(0),
                org_dsqr(0),
                advected(0),
                allocated(0)
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
                    memory::kind<memory::global>::release_as(original,allocated);
                    org_diff = 0;
                    org_dsqr = 0;
                    advected = 0;
                }


                float *original;
                float *org_diff;
                float *org_dsqr;
                float *advected;
                size_t allocated;

                void allocate()
                {
                    assert(allocated<=0);
                    assert(0==original);
                    allocated = 4*items;
                    original  = memory::kind<memory::global>::acquire_as<float>(allocated);
                    org_diff  = original + items;
                    org_dsqr  = org_diff + items;
                    advected  = org_dsqr + items;
                }

                void load( const pixmapf &src ) throw()
                {
                    assert(original);
                    assert(org_diff);
                    assert(org_dsqr);
                    assert(advected);
                    assert(src.contains(*this));
                    size_t k       = 0;
                    float  average = 0;
                    for(unit_t y=upper.y;y>=lower.y;--y)
                    {
                        const pixmapf::row &src_y = src[y];
                        for(unit_t x=upper.x;x>=lower.x;--x)
                        {
                            const float tmp = src_y[x];
                            original[k++] = tmp;
                            average += tmp;
                        }
                    }
                    assert(items==k);
                    average/=items;
                    while(k>0)
                    {
                        --k;
                        const float del = original[k] - average;
                        org_diff[k] = del;
                        org_dsqr[k] = del*del;
                    }
                }

                float correlate_with(const pixmapf &tgt, const vertex delta ) const
                {
                    assert(delta.x>=dmin.x);
                    assert(delta.x<=dmax.x);
                    assert(delta.y>=dmin.y);
                    assert(delta.y<=dmax.y);

                    //-- first loop
                    const vertex _ini = lower+delta;
                    const vertex _end = upper+delta;
                    size_t k = 0;
                    float  adv = 0;
                    for(unit_t y=_ini.y;y<=_end.y;++y)
                    {
                        const pixmapf::row &tgt_y = tgt[y];
                        for(unit_t x=_ini.x;x<=_end.x;++x)
                        {
                            const float tmp = tgt_y[x];
                            advected[k++] = tmp;
                            adv += tmp;
                        }
                    }

                    // -- second loop
                    

                    return 0;
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

                size_t allocated() const throw()
                {
                    const _Zones &self = *this;
                    size_t sum = 0;
                    for(unit_t j=0;j<H;++j)
                    {
                        for(unit_t i=0;i<W;++i)
                        {
                            sum += self[j][i].allocated;
                        }
                    }
                    return sum;
                }


                virtual ~Zones() throw() {}
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Zones);
            };
        }
    }
}



#endif

