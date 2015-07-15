#ifndef YOCTO_GFX_OPS_HISTOGRAM_INCLUDED
#define YOCTO_GFX_OPS_HISTOGRAM_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/type/ints.hpp"
#include <cstring>

namespace yocto
{
    namespace gfx
    {



        //!
        template <size_t BYTES>
        class histogram
        {
        public:
            typedef typename unsigned_int<BYTES>::type uint_t;
            static  const size_t                       bins = unsigned_int<BYTES>::maximum;

            size_t classes;
            double count[bins];
            uint_t bin[bins]; //! 0..classes-1
            double cdf[bins]; //! 0..classes-1

            inline void reset() throw()
            {
                classes=0;
                memset(count,0,sizeof(count));
                memset(bin,0,sizeof(bin));
                memset(cdf,0,sizeof(cdf));
            }


            explicit histogram() throw() :
            classes(0),
            count(),
            bin(),
            cdf()
            {
                reset();
            }

            virtual ~histogram() throw()
            {
                reset();
            }

            template <typename T>
            void build_from( const pixmap<T> &px, uint_t (*addr2word)(const T&) ) throw()
            {
                assert(addr2word);
                const unit_t w = px.w;
                const unit_t h = px.h;
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Rj = px[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        const uint_t w = addr2word( Rj[i] );
                        ++count[w];
                    }
                }
                build_cdf();
            }

            void build_cdf() throw()
            {
                classes = 0;
                for(uint_t i=0;i<bins;++i)
                {
                    const double ni = count[i];
                    if(ni>0)
                    {
                        bin[classes] = i;
                        cdf[classes] = ni;
                        ++classes;
                    }
                }
                for(unit_t i=1;i<classes;++i)
                {
                    cdf[i] += cdf[i-1];
                }
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
        };

        typedef histogram<1> hist8;
        typedef histogram<2> hist16;
    }
}

#endif
