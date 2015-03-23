#ifndef YOCTO_GEMS_IO_CRYSTAL_INCLUDED
#define YOCTO_GEMS_IO_CRYSTAL_INCLUDED 1

#include "yocto/gems/atoms.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace gems
    {

        class crystal
        {
        public:
            static unit_t num_per_mesh(const double x) throw();
            //! count how many particles, closed packed in a box
            /**
             \param cr  computed number of particles per axis
             \param n   the desired number of particles
             \param bx,by,bz the box sizes
             \return 4 * cr.x * cr.y * cr.z
             */
            static size_t close_packed_count(coord_t     &cr,
                                             size_t       n,
                                             const double bx,
                                             const double by,
                                             const double bz) throw();

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
                return close_packed_count(cr,n,box.x,box.y,box.z);
            }

            typedef void (*build_proc)(size_t i, double x,double y,double z,void *args);
            //! create coordinates for a FCC packing
            /**
             \param cr computed number of particles per axis => 4 * cr.x * cr.y * cr.z coordinates
             \param bx,by,bz the box sizes
             \param proc called with 0..closed_pack_count-1
             */
            static  void build_fcc(const coord_t &cr,
                                   const double   bx,
                                   const double   by,
                                   const double   bz,
                                   build_proc     proc,
                                   void          *args
                                   ) throw();

            template <typename T>
            static inline void build_fcc(atoms<T>      &Atoms,
                                         const coord_t &cr,
                                         const v3d<T>  &box) throw()
            {
                build_fcc(cr, box.x, box.y, box.z, build_atom<T>,&Atoms);
            }


        private:
            template <typename T>
            static inline void build_atom(size_t indx, double x, double y, double z,void *args) throw()
            {
                assert(args);
                atoms<T> &Atoms = *(atoms<T> *)args;
                v3d<T>   &r     = Atoms[indx].r;
                r.x = T(x);
                r.y = T(y);
                r.z = T(z);
            }
        };
        
    }
}


#endif
