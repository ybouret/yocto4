#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/types.hpp"

#include <iostream>

namespace yocto
{
    namespace gfx
    {

        histogram:: histogram() throw() :
        classes(0),
        count(),
        bin(),
        cdf()
        {
            reset();
        }

        histogram:: ~histogram() throw()
        {
        }

        void histogram:: reset() throw()
        {
            classes=0;
            memset(count,0,sizeof(count));
            memset(bin,0,sizeof(bin));
            memset(cdf,0,sizeof(cdf));
            memset(lut,0,sizeof(lut));
        }



        void histogram:: build_cdf() throw()
        {
            classes = 0;
            memset(bin,0,sizeof(bin));
            memset(cdf,0,sizeof(cdf));

            for(size_t i=0;i<bins;++i)
            {
                const double ni = count[i];
                if(ni>0)
                {
                    bin[classes] = i;
                    cdf[classes] = ni;
                    ++classes;
                }
            }

            for(size_t i=1;i<classes;++i)
            {
                cdf[i] += cdf[i-1];
            }

            if(classes>1)
            {
                const double cmin  = cdf[0];
                const double cmax  = cdf[classes-1];
                std::cerr << "bin: from " << int(bin[0]) << " to " << int(bin[classes-1]) << std::endl;
                const double delta = cmax-cmin;
                for(size_t i=0;i<classes;++i)
                {
                    cdf[i] = (cdf[i]-cmin)/delta;
                }
            }
        }


        double histogram:: icdf(const double y, const double gam) const throw()
        {
            assert(classes>1);
            const size_t top = classes-1;
            assert(bin[top]>bin[0]);

            if(y<=0.0)
            {
                return 0;
            }
            else
            {
                if(y>=1.0)
                {
                    return 255;
                }
                else
                {
                    size_t jlo = 0;
                    size_t jhi = top;
                    while(jhi-jlo>1)
                    {
                        const size_t jmid = (jlo+jhi)>>1;
                        const double ymid = pow(cdf[jmid],gam);
                        if(y<ymid)
                        {
                            jhi = jmid;
                        }
                        else
                        {
                            jlo = jmid;
                        }
                    }
                    const double ylo = (jlo<=0 ? 0 : pow(cdf[jlo],gam));
                    const double yhi = pow(cdf[jhi],gam);
                    const double xlo = bin[jlo];
                    const double xhi = bin[jhi];
                    // raw result
                    const double x0  = xlo + (xhi-xlo)*(y-ylo)/(yhi-ylo);

                    // expand result
                    const double b0 = bin[0];
                    return clamp<double>(0,255.0*(x0-b0)/(bin[top]-b0),255);
                }
            }

        }

        void histogram:: build_lut(const double gam) throw()
        {
            switch(classes)
            {
                case 0:
                    memset(lut,0,sizeof(lut));
                    return;

                case 1:
                    memset(lut,0xff,sizeof(lut));
                    return;

                default:
                    break;
            }
            //const size_t top = classes-1;

            static const double den = bins-1;
            for(size_t j=0;j<bins;++j)
            {
                const double y = j/den;
                const double x = icdf(y,gam);
                lut[j] = uint8_t( floor(x+0.5) );
            }
        }

    }

}

namespace yocto
{
    namespace gfx
    {

        static inline
        double get_full_variance(const double *count, size_t t)
        {
            assert(t<histogram::bins);

            //__________________________________________________________________
            //
            // background average
            //__________________________________________________________________

            size_t nb   = 0;
            double mb   = 0;
            for(size_t i=0;i<t;++i)
            {
                const size_t n = size_t(count[i]);
                nb += n;
                mb += double(i) * n;
            }

            //__________________________________________________________________
            //
            // background variance
            //__________________________________________________________________
            double vb = 0;
            if(nb>0)
            {
                mb /= nb;
                for(size_t i=0;i<t;++i)
                {
                    const double d = double(i) - mb;
                    vb += count[i] * d*d;
                }
                vb /= nb;
            }

            //__________________________________________________________________
            //
            // foreground average
            //__________________________________________________________________
            size_t nf = 0;
            double mf = 0;
            for(size_t i=t;i<histogram::bins;++i)
            {
                const size_t n = size_t(count[i]);
                nf += n;
                mf += double(i) * n;
            }

            //__________________________________________________________________
            //
            // foreground variance
            //__________________________________________________________________
            double vf = 0;
            if(nf>0)
            {
                mf /= nf;
                for(size_t i=t;i<histogram::bins;++i)
                {
                    const double d = double(i) - mf;
                    vf += count[i] * d*d;
                }
                vf /= nf;
            }

            const size_t ntot = nb + nf;
            if(ntot>0)
            {
                return (nb*vb+nf*vf)/ntot;
            }
            else
            {
                return 0;
            }
        }
        
        size_t histogram:: threshold() const throw()
        {
            size_t ans = 0;
            double opt = get_full_variance(count,ans);
            for(size_t i=1;i<bins;++i)
            {
                const double tmp = get_full_variance(count,i);
                if(tmp<opt)
                {
                    ans = i;
                    opt = tmp;
                }
            }
            return ans;
        }
        
    }
    
}


