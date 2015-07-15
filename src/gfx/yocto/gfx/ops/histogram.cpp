#include "yocto/gfx/ops/histogram.hpp"

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
            const size_t top = classes-1;

            {
                const size_t head = bin[0];
                for(size_t i=0;i<=head;++i)
                {
                    lut[i] = 0;
                }
            }

            {
                const size_t last = bin[top];
                for(size_t i=last;i<bins;++i)
                {
                    lut[i] = 255;
                }
            }

            size_t jlo = bin[0];
            double clo = 0;
            for(size_t i=1;i<classes;++i)
            {
                const size_t jhi = bin[i];
                const double chi = cdf[i];
                const double dj  = double(jhi - jlo);
                const double dc  = chi - clo;
                for(size_t j=jlo;j<jhi;++j)
                {
                    const double cj = clo + ((j-jlo)/dj) * dc;
                }
                jlo = jhi;
                clo = chi;
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


