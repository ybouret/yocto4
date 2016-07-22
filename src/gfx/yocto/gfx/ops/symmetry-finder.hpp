#ifndef YOCTO_GFX_OPS_SYMMETRY_FINDER_INCLUDED
#define YOCTO_GFX_OPS_SYMMETRY_FINDER_INCLUDED 1

#include "yocto/gfx/ops/geometry.hpp"
#include "yocto/gfx/ops/diff.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/trigconv.hpp"

namespace yocto
{
    namespace gfx
    {

        template <typename T>
        class symmetry_finder
        {
        public:
            inline  symmetry_finder(pixmap<T>       &user_target,
                                    const pixmap<T> &user_source,
                                    xpatches        &user_xps) throw() :
            target(user_target),
            source(user_source),
            xps(user_xps),
            atmp(2),
            I0( diff.L2(source,xps) ),
            scalarF(this, & symmetry_finder::__scalar ),
            vectorF(this, & symmetry_finder::__vector )
            {
            }

            inline ~symmetry_finder() throw() {}


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(symmetry_finder);
            geometry          geom;
            difference        diff;
            pixmap<T>        &target;
            const pixmap<T>  &source;
            xpatches         &xps;
            vector<float>     atmp;
            inline float      __scalar(const array<float> &aorg)
            {
                assert(aorg.size()>=2);
                const float alpha = aorg[1];
                const float a     = cos(alpha);
                const float b     = sin(alpha);
                const float c     = aorg[2];
                // compute symmetry
                geom.symmetry(target,source,a,b,c,xps);

                // compute difference
                return diff.L2(target,source,xps)/I0;
            }

            inline float computeValue() const throw()
            {
                return diff.L2(target,source,xps);
            }

            inline void __vector(array<float> &drvs, const array<float> &aorg)
            {
                static const float delta_deg   = 1.0f;
                static const float scale_alpha = math::Deg2Rad(delta_deg+delta_deg);

                // one degree trial
                {
                    const float alpha_deg = math::Rad2Deg(aorg[1]);
                    atmp[2] = aorg[2];
                    atmp[1] = math::Deg2Rad( alpha_deg + delta_deg );
                    const float vp = __scalar(atmp);
                    atmp[1] = math::Deg2Rad( alpha_deg - delta_deg );
                    const float vm = __scalar(atmp);
                    drvs[1] = (vp-vm)/scale_alpha;
                }

                static const float delta_c = 1.0f;
                static const float scale_c = delta_c+delta_c;
                // one pixel trial
                {
                    atmp[1] = aorg[1];
                    const float c = aorg[2];
                    atmp[2] = c + delta_c;
                    const float vp = __scalar(atmp);
                    atmp[2] = c - delta_c;
                    const float vm = __scalar(atmp);
                    drvs[2] = (vp-vm)/scale_c;
                }
            }


        public:
            const float                        I0;
            math::numeric<float>::scalar_field scalarF;
            math::numeric<float>::vector_field vectorF;

            inline bool __callback(const array<float> &aorg)
            {
                vector<float> drvs(2);
                __vector(drvs,aorg);
                std::cerr << "a=" << aorg << " => " << __scalar(aorg) << ", grad=" << drvs << std::endl;
                //std::cerr << "aorg=" << aorg << std::endl;
                return true;
            }

        };

    }
}

#endif
