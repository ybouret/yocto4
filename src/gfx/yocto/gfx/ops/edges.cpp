#include "yocto/gfx/ops/edges.hpp"

namespace yocto
{
    namespace gfx
    {
        Edges:: Edges(const unit_t W, const unit_t H) :
        pixmap<float>(W,H),
        I(W,H),
        S(W,H),
        src(0),
        global()
        {
        }

        Edges:: ~Edges() throw()
        {

        }

        const vertex Edges::shift[9] =
        {
            vertex(-1,-1), vertex(-1,0), vertex(-1,1),
            vertex( 0,-1), vertex( 0,0), vertex( 0,1),
            vertex( 1,-1), vertex( 1,0), vertex( 1,1)
        };

        const float Edges::sobel1[9] =
        {
            1,0,-1,
            2,0,-2,
            1,0,-1
        };

        const float Edges::sobel2[9] =
        {
            1,2,1,
            0,0,0,
            -1,-2,-1
        };

        void Edges:: buildE( xpatch &xp, lockable & ) throw()
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

        void Edges:: finalize(xpatch &xp, lockable & ) throw()
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


namespace yocto
{
    namespace gfx
    {
        EdgeDetector:: ~EdgeDetector() throw()
        {
        }

        EdgeDetector:: EdgeDetector(const unit_t W, const unit_t H) :
        pixmap<float>(W,H),
        A(W,H), B(W,H),
        E(W,H),
        src(0),
        ddx(0),
        ddy(0),
        Gmax(0)
        {
        }


#if 0
        void EdgeDetector:: normalize(xpatch &xp, lockable &) throw()
        {
            assert(Gmax>0);
            pixmap<float> &G  = *this;
            const float   fac = 1.0f/Gmax;
            for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
            {
                for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                {
                    G[y][x] *= fac;
                }
            }
        }
#endif


        void EdgeDetector:: non_maxima_suppress(xpatch &xp,  lockable &) throw()
        {
            assert(Gmax>0);
            pixmap<float> &G   = *this;
            const float    fac = 1.0f/Gmax;
            for(unit_t y=xp.upper.y,yl=y-1,yu=y+1;y>=xp.lower.y;--y,--yl,--yu)
            {
                const bitmap::position_type ypos = y_position(y);
                for(unit_t x=xp.upper.x,xl=x-1,xu=x+1;x>=xp.lower.x;--x,--xl,--xu)
                {
                    const bitmap::position_type xpos = x_position(x);
                    const float                 G0   = G[y][x];
                    bool                        keep = false;
                    switch(A[y][x])
                    {
                        case DIR_VERT: {
                            switch( ypos )
                            {
                                case bitmap::at_lower: keep = (G0>=G[yu][x]); break;
                                case bitmap::at_upper: keep = (G0>=G[yl][x]); break;
                                case bitmap::is_inner: keep = (G0>=G[yu][x] && G0>=G[yl][x]); break;
                            }
                        } break;

                        case DIR_HORZ: {
                            switch( xpos )
                            {
                                case bitmap::at_lower: keep = (G0>=G[y][xu]); break;
                                case bitmap::at_upper: keep = (G0>=G[y][xl]); break;
                                case bitmap::is_inner: keep = (G0>=G[y][xu] && G0>=G[y][xl]); break;
                            }
                        } break;

                        case DIR_DIAG_RIGHT: {
                            
                        } break;

                        default:
                            break;
                    }
                    if(keep)
                    {
                        E[y][x] = G0*fac;
                    }
                    else
                    {
                        E[y][x] = 0;
                    }
                }
            }
        }


    }
}
