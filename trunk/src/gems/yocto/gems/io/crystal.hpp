#ifndef YOCTO_GEMS_IO_CRYSTAL_INCLUDED
#define YOCTO_GEMS_IO_CRYSTAL_INCLUDED 1

#include "yocto/gems/atom.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace gems
    {

        struct crystal
        {

            template <typename T>
            static inline unit_t num_per_mesh(const T x) throw()
            {
                assert(x>=0);
                const unit_t ans = unit_t(ANINT(x));
                return (ans<1? 1 : ans);
            }

            //! count how many particles, closed packed in a box
            /**
             \param cr  computer number of particles per axis
             \param n   the desired number of particles
             \param box the box size
             \return 4 * cr.x * cr.y * cr.z
             */
            template <typename T>
            static inline size_t close_packed_count( coord_t &cr, size_t n, const v3d<T> &box ) throw()
            {
                assert(n>0);
                assert(box.x>0);
                assert(box.y>0);
                assert(box.z>0);
                const T d     = (box.x*box.y*box.z)/n;
                const T alpha = math::Pow(T(1)/(d*T(4)), T(1)/T(3));
                cr.x = num_per_mesh(box.x*alpha);
                cr.y = num_per_mesh(box.y*alpha);
                cr.z = num_per_mesh(box.z*alpha);

                return 4 * cr.x * cr.y * cr.z;
            }
        };

    }
}


#endif
