#include "yocto/gfx/ops/edges.hpp"

namespace yocto
{
    namespace gfx
    {
        edges:: edges(const unit_t W, const unit_t H) :
        pixmap<float>(W,H),
        I(W,H),
        S(W,H),
        src(0),
        global()
        {
        }

        edges:: ~edges() throw()
        {

        }

        const vertex edges::shift[9] =
        {
            vertex(-1,-1), vertex(-1,0), vertex(-1,1),
            vertex( 0,-1), vertex( 0,0), vertex( 0,1),
            vertex( 1,-1), vertex( 1,0), vertex( 1,1)
        };

        const float edges::sobel1[9] =
        {
            1,0,-1,
            2,0,-2,
            1,0,-1
        };

        const float edges::sobel2[9] =
        {
            1,2,1,
            0,0,0,
            -1,-2,-1
        };

        void edges:: buildE( xpatch &xp, lockable & ) throw()
        {
            const unit_t    ymin = xp.lower.y;
            const unit_t    ymax = xp.upper.y;
            const unit_t    xmin = xp.lower.x;
            const unit_t    xmax = xp.upper.x;
            info            local;
            pixmap<float>  &E    = *this;
            float           arr[9];

            for(unit_t y=ymax;y>=ymin;--y)
            {
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const vertex here(x,y);
                    float sum1 = 0; //! for sobel1
                    float sum2 = 0; //! for sobel2
                    float Iave = 0; //! for I average
                    for(size_t k=0;k<9;++k)
                    {
                        const vertex probe = here + shift[k];
                        float        II    = 0;
                        if(has(probe))
                        {
                            II = I[probe];
                        }
                        else
                        {
                            II = I[here];
                        }
                        sum1 += sobel1[k] * II;
                        sum2 += sobel2[k] * II;
                        arr[k] = II;
                        Iave  += II;
                    }

                    // build edge field
                    const float E_val = sqrtf(sum1*sum1+sum2*sum2);
                    E[y][x] = E_val;
                    local.Emax = max_of(local.Emax,E_val);

                    //build std dev field
                    Iave /= 9;
                    float S_val = 0;
                    for(register size_t k=0;k<9;++k)
                    {
                        const float delta = arr[k] - Iave;
                        S_val += delta*delta;
                    }
                    S_val = sqrtf(0.125f*S_val);
                    S[y][x] = S_val;
                    local.Smax = max_of(local.Smax,S_val);
                }
            }
            xp.as<info>() = local;
        }

        void edges:: finalize(xpatch &xp, lockable & ) throw()
        {
            const unit_t         ymin     = xp.lower.y;
            const unit_t         ymax     = xp.upper.y;
            const unit_t         xmin     = xp.lower.x;
            const unit_t         xmax     = xp.upper.x;
            const info           local(global);
            pixmap<float>       &E     = *this;

            for(unit_t y=ymax;y>=ymin;--y)
            {
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    E[y][x] /= local.Emax;
                    S[y][x] /= local.Smax;
                }
            }
        }

    }
}
