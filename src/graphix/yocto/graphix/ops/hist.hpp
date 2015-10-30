#ifndef YOCTO_GRAPHIX_OPS_INCLUDED
#define YOCTO_GRAPHIX_OPS_INCLUDED 1

#include "yocto/graphix/rawpix.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace graphix
    {

        class histogram
        {
        public:
            static const size_t bins=256;
            typedef uint64_t    word_type;

            word_type count[256];

            explicit histogram() throw();
            virtual ~histogram() throw();

            //! for parallel code
            template <typename T>
            inline void update(const pixmap<T> &pxm, const patch &area) throw()
            {
                assert(pxm.contains(area));
                for(unit_t j=area.lower.y;j<=area.upper.y;++j)
                {
                    const typename pixmap<T>::row &pj = pxm[j];
                    for(unit_t i=area.lower.x;i<=area.upper.x;++i)
                    {
                        const T &px = pj[i];
                        ++count[ project<T>(px) ];
                    }
                }
            }

            void reset() throw();                   //! everyting to 0
            void merge(const histogram &h) throw(); //!< append bins to bins
            void save(const string &hname) const;   //!< save to check

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
        };

        class histogram_patch : public patch
        {
        public:
            const void *src;
            histogram   H;

            explicit histogram_patch(const patch &p) throw();
            virtual ~histogram_patch() throw();

            template <typename T>
            void compute() throw()
            {
                assert(src);
                H.reset();
                const pixmap<T> &pxm = *static_cast<const pixmap<T>*>(src);
                H.update<T>(pxm,*this);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram_patch);
        };



    }
}

#endif
