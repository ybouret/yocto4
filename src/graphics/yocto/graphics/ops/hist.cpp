#include "yocto/graphics/ops/hist.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace graphics
    {

        histogram:: ~histogram() throw()
        {
        }

        histogram:: histogram() throw() : count()
        {
            reset();
        }

#define Y_GFX_HIST_RESET(I) arr[I] = 0
        void histogram:: __reset(word_type *arr) throw()
        {
            assert(arr);
            YOCTO_LOOP_FUNC_(bins,Y_GFX_HIST_RESET,0);
        }

        void histogram:: reset() throw()
        {
            __reset(count);
        }


        void histogram:: save(const string &hname) const
        {
            ios::wcstream fp(hname);
            for(size_t i=0;i<bins;++i)
            {
                fp("%u %g\n", unsigned(i), double(count[i]));
            }
        }
        
        void histogram:: save(const char *hname) const
        {
            const string id(hname);
            save(id);
        }


        void histogram::create(patches &hp, const graphics::patch &surface, threading::engine *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(hp,cpus,surface,true);
        }

        void histogram::finish(const patches &patches, threading::engine *server)
        {
            if(server) server->flush();
            for(size_t i=patches.size();i>0;--i)
            {
                const patch &sub = patches[i];
                for(size_t j=0;j<bins;++j)
                {
                    count[j] += sub.count[j];
                }
            }
        }

        bool histogram:: build_cdf(vector<cbin> &cdf, uint8_t *lut) const
        {
            cdf.free();
            cdf.ensure(256);
            word_type curr = 0;
            for(size_t i=0;i<256;++i)
            {
                const word_type tmp = count[i];
                if(tmp>0)
                {
                    curr += tmp;
                    cdf.append<size_t,double>(i,double(curr));
                }
            }
            const size_t n = cdf.size();

            if(n>=2)
            {
                if(lut)
                {
                    std::cerr << "COMPUTING LUT" << std::endl;
                    memset(lut,0,256);
                    lut[255] = 255;
                    const cbin      cmin      = cdf.front();
                    const cbin      cmax      = cdf.back();
                    const size_t    xmin      = cmin.x;
                    const size_t    xmax      = cmax.x;
                    const size_t    xlen      = xmax-xmin;
                    const double    ylen      = cmax.y-cmax.x;
                    for(size_t x=xmin;x<=xmax;++x)
                    {
                        //! the should be value
                        const double yi = cmin.y + ((x-xmin)*ylen)/xlen;
                        size_t jlo = 1;
                        size_t jhi = n;
                        while(jhi-jlo>1)
                        {
                            const size_t jmid = (jlo+jhi)>>1;
                            const double ymid = cdf[jmid].y;
                            if(ymid<yi)
                            {
                                jlo = jmid;
                            }
                            else
                            {
                                jhi = jmid;
                            }
                        }
                        assert(1==jhi-jlo);
                        const cbin   lo = cdf[jlo];
                        const cbin   hi = cdf[jhi];
                        // raw corresponding fit
                        const double xi = double(lo.x) + (yi-lo.y) * double(hi.x-lo.x) / (hi.y-lo.y);

                        // expanded fit
                        const double alpha = (xi-xmin)/xlen;
                        const size_t xx    = gist::float2byte(float(alpha));
                        //std::cerr << "yi=" << yi << "=>" << xi <<  " -> " << xx << std::endl;
                        lut[x] = xx;
                    }


                }
                return true;
            }
            return false;
        }

    }

}



namespace yocto
{
    namespace graphics
    {
        histogram::patch:: ~patch() throw()
        {
        }

        histogram::patch:: patch(const graphics::patch &p) throw() :
        graphics::patch(p),
        src(0),
        count()
        {
        }       
	
	histogram::patch:: patch(const patch &p) throw() :
	graphics::patch(p),
	src(p.src),
	count()
	{
	}
	 
        
    }
    
}


namespace yocto
{
    namespace graphics
    {
        static inline
        double get_full_variance(const histogram::word_type *count, size_t t)
        {
            assert(t<histogram::bins);
            typedef histogram::word_type word_t;

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
            for(size_t i=t;i<histogram::bins;++i)
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
                for(size_t i=t;i<histogram::bins;++i)
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




