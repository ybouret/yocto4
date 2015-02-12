#include "yocto/gems/io/crystal.hpp"

namespace yocto
{

    namespace gems
    {
        unit_t crystal:: num_per_mesh(const double x) throw()
        {
            assert(x>=0);
            const unit_t ans = unit_t(ANINT(x));
            return (ans<1? 1 : ans);
        }

        size_t crystal:: close_packed_count(coord_t     &cr,
                                             size_t       n,
                                             const double bx,
                                             const double by,
                                             const double bz) throw()
        {
            assert(n>0);
            assert(bx>0);
            assert(by>0);
            assert(bz>0);
            const double d     = (bx*by*bz)/n;
            const double alpha = pow(1.0/(d*4),1.0/3.0);//math::Pow(T(1)/(d*T(4)), T(1)/T(3));
            cr.x = num_per_mesh(bx*alpha);
            cr.y = num_per_mesh(by*alpha);
            cr.z = num_per_mesh(bz*alpha);
            return 4 * cr.x * cr.y * cr.z;
        }

        void crystal:: build_fcc(const coord_t &cr,
                                 const double   bx,
                                 const double   by,
                                 const double   bz,
                                 build_proc     proc,
                                 void          *args) throw()
        {
            assert(cr.x>0);
            assert(cr.y>0);
            assert(cr.z>0);
            assert(proc);
            const size_t n = 4 * cr.x * cr.y * cr.z;

            const double dx = bx/cr.x; const double dx2 = dx*0.5;
            const double dy = by/cr.y; const double dy2 = dy*0.5;
            const double dz = bz/cr.z; const double dz2 = dz*0.5;

            const v3d<double> r[4] =
            {
                v3d<double>(0.0,0.0,0.0), //!< sublattice A
                v3d<double>(dx2,dy2,0.0), //!< sublattice B
                v3d<double>(0.0,dy2,dz2), //!< sublattice C
                v3d<double>(dx2,0.0,dz2)  //!< sublattice D
            };
            size_t m = 0;

            // Repeat sublattices
            for(size_t k=0; k < cr.z; ++k )
            {
                const double z = k * dz;
                for( size_t j=0; j < cr.y; ++j )
                {
                    const double y = j * dy;
                    for(size_t i=0; i < cr.x;  ++i )
                    {
                        const double x = i * dx;
                        for(size_t iref=0; iref < 4; ++ iref )
                        {
                            v3d<double> pos(r[iref]);
                            pos.x += x;
                            pos.y += y;
                            pos.z += z;
                            proc(m,pos.x,pos.y,pos.z,args);
                            ++m;
#if 0
                            const size_t p = iref+m;
                            assert( p < n );
                            r[p].x = r[iref].x + x;
                            r[p].y = r[iref].y + y;
                            r[p].z = r[iref].z + z;
#endif
                        }
                    }
                }
            }
            assert( m == n );

        }

    }
    
}
