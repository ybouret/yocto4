#ifndef YOCTO_GFX_OPS_HISTOGRAM_INCLUDED
#define YOCTO_GFX_OPS_HISTOGRAM_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {



        class histogram
        {
        public:
            static const size_t max_bins = 256;
            unsigned            bins; //!< for cdf
            size_t              count[max_bins];
            uint8_t             bin[max_bins]; //! 0..bins-1
            double              cdf[max_bins]; //! 0..bins-1
            
            explicit histogram():
            bins(0),
            count(),
            bin(),
            cdf()
            {
                reset();
            }

            virtual ~histogram() throw()
            {

            }

            inline histogram(const histogram &H) throw() : count()
            {
                unsafe_dup(H);
            }

            inline histogram & operator=(const histogram &H) throw()
            {
                duplicate(H);
                return *this;
            }

            inline void reset() throw()
            {
                bins = 0;
                memset(count,0,sizeof(count));
                memset(bin,0,sizeof(bin));
                memset(cdf,0,sizeof(cdf));
                
            }

            inline void duplicate( const histogram &H ) throw()
            {
                if(this != &H )
                {
                    unsafe_dup(H);
                }
            }

            
            
            template <typename U>
            void build_for( const pixmap<U> &pxm, uint8_t (*data2byte)(const U &)  ) throw()
            {
                assert(data2byte);
                reset();
                const unit_t w = pxm.w;
                const unit_t h = pxm.h;
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<U>::row &pxr = pxm[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        const uint8_t u =data2byte(pxr[i]);
                        ++count[u];
                    }
                }
                build_cdf();
            }
            
            inline void build_cdf() throw()
            {
                bins = 0;
                for(unsigned i=0;i<bins;++i)
                {
                    
                }
            }

        private:
            inline void unsafe_dup(const histogram &H) throw()
            {
                assert(&H!=this);
                bins = H.bins;
                memcpy(count,H.count,sizeof(count));
                memcpy(bin,H.bin,sizeof(bin));
                memcpy(cdf,H.cdf,sizeof(cdf));
            }
            
        };
        
    }
}

#endif
