#include "yocto/graphics/ops/histogram.hpp"
#include "yocto/ios/ocstream.hpp"

#include <cstring>

namespace yocto
{
    namespace graphics
    {

        Histogram:: Histogram() throw() :
        count(),
        src(0)
        {
            reset();
        }

        Histogram:: ~Histogram() throw()
        {
        }


        void Histogram:: reset() throw()
        {
            memset(count,0,sizeof(count));
        }

        void Histogram:: save(const string &hname) const
        {
            ios::wcstream fp(hname);
            for(size_t i=0;i<bins;++i)
            {
                fp("%u %g\n", unsigned(i), double(count[i]));
            }
        }

        void Histogram:: save(const char *hname) const
        {
            const string id(hname);
            save(id);
        }

        void Histogram:: collect( const Histogram &H ) throw()
        {
            for(size_t i=0;i<bins;++i)
            {
                count[i] += H.count[i];
            }
        }


    }

}

namespace yocto
{
    namespace graphics
    {
        static inline
        double get_full_variance(const Histogram::word_type *count, size_t t)
        {
            assert(t<Histogram::bins);
            typedef Histogram::word_type word_t;

            //__________________________________________________________________
            //
            // background average
            //__________________________________________________________________

            word_t nb   = 0;
            double mb   = 0;
            for(size_t i=0;i<t;++i)
            {
                const word_t n = count[i];
                nb += n;
                mb += double(i) * double(n);
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
                    vb += double(count[i]) * d*d;
                }
                vb /= nb;
            }

            //__________________________________________________________________
            //
            // foreground average
            //__________________________________________________________________
            word_t nf = 0;
            double mf = 0;
            for(size_t i=t;i<Histogram::bins;++i)
            {
                const word_t n = count[i];
                nf += n;
                mf += double(i) * double(n);
            }

            //__________________________________________________________________
            //
            // foreground variance
            //__________________________________________________________________
            double vf = 0;
            if(nf>0)
            {
                mf /= nf;
                for(size_t i=t;i<Histogram::bins;++i)
                {
                    const double d = double(i) - mf;
                    vf += double(count[i]) * d*d;
                }
                vf /= nf;
            }

            const word_t ntot = nb + nf;
            if(ntot>0)
            {
                return (nb*vb+nf*vf)/ntot;
            }
            else
            {
                return 0;
            }
        }


        size_t Histogram:: threshold() const throw()
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

