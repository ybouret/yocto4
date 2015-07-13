#ifndef YOCTO_GFX_OPS_HISTOGRAM_INCLUDED
#define YOCTO_GFX_OPS_HISTOGRAM_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {



        template <typename T>
        class histogram
        {
        public:
            static const size_t bins = 256;
            T               count[bins];
            double          cumul[bins];

            explicit histogram() throw() :
            count()
            {
                reset();
            }

            virtual ~histogram() throw()
            {

            }

            inline histogram(const histogram &H) throw() : count()
            {
                __dup(H);
            }

            inline histogram & operator=(const histogram &H) throw()
            {
                duplicate(H);
                return *this;
            }

            inline void reset() throw()
            {
                memset(count,0,sizeof(count));
                memset(cumul,0,sizeof(cumul));
            }

            inline void duplicate( const histogram &H ) throw()
            {
                if(this != &H )
                {
                    __dup(H);
                }
            }

            inline void build_cdf() throw()
            {
                cumul[0] = count[0];
                for(int i=1;i<bins;++i)
                {
                    cumul[i] = cumul[i-1] + count[i];
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
                        ++count[ data2byte(pxr[i]) ];
                    }
                }
                build_cdf();
            }

        private:
            inline void __dup(const histogram &H) throw()
            {
                assert(H!=&this);
                memcpy(count,H.count,sizeof(count));
                memcpy(cumul,H.cumul,sizeof(cumul));
            }
            
        };
        
    }
}

#endif
