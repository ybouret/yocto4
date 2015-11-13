#ifndef YOCTO_SPADE_FORMAT_STL_INCLUDED
#define YOCTO_SPADE_FORMAT_STL_INCLUDED 1

#include "yocto/spade/types.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/ios/ostream.hpp"
#include <cerrno>

namespace yocto
{
    namespace spade
    {
        struct stl
        {

            //! an oriented facet
            template <typename T>
            class facet
            {
            public:
                typedef point3d<T> vtx;

                const vtx  *v1;
                const vtx  *v2;
                const vtx  *v3;
                const vtx   n;

                inline facet(const facet &other) throw() :
                v1(other.v1),
                v2(other.v2),
                v3(other.v3),
                n(other.n)
                {
                }

                inline facet(const vtx &p1,
                             const vtx &p2,
                             const vtx &p3,
                             const vtx &inside) :
                v1(&p1),
                v2(&p2),
                v3(&p3),
                n()
                {
                    if(v1==v2||v1==v3||v2==v3) throw libc::exception(EINVAL,"same vertices in stl::facet");
                    const vtx v12(*v1,*v2);
                    const vtx v13(*v1,*v3);
                    vtx  &s = (vtx&)n;
                    s = (v12^v13);
                    const T s2 = s*s;
                    if(s2<=0) throw libc::exception(EINVAL,"parallel edged in stl::facet");
                    s /= math::Sqrt(s2);
                    const vtx bar = (p1+p2+p3)/T(3);
                    const vtx seg(inside,bar);
                    if(s*seg<=0)
                    {
                        s = -s;
                        cswap(v1,v2);
                    }
                }

                inline virtual ~facet() throw() {}


            private:
                YOCTO_DISABLE_ASSIGN(facet);
            };

            template <typename T> static inline
            void close_contour(sequence< facet<T> >                  &facets,
                               const array< typename facet<T>::vtx > &contour,
                               const typename facet<T>::vtx          &top,
                               const typename facet<T>::vtx          &inside)
            {
                const size_t n = contour.size();
                for(size_t i=1;i<=n;++i)
                {
                    const typename facet<T>::vtx &p2 = contour[i];
                    const typename facet<T>::vtx &p3 = contour[i<n?i+1:1];
                    const facet<T> f(top,p2,p3,inside);
                    facets.push_back(f);
                }
            }

            
            template <typename T>
            static inline void write_vtx(ios::ostream &fp, const point3d<T> &v )
            {
                fp("%e %e %e",v.x,v.y,v.z);
            }

            template <typename T>
            static inline void emit(ios::ostream &fp, const facet<T> &f)
            {
                fp << " facet normal "; write_vtx(fp,f.n); fp << "\n";
                fp << "  outer loop\n";
                fp << "   vertex "; write_vtx(fp, *(f.v1) ); fp << "\n";
                fp << "   vertex "; write_vtx(fp, *(f.v2) ); fp << "\n";
                fp << "   vertex "; write_vtx(fp, *(f.v3) ); fp << "\n";
                fp << "  endloop\n";
                fp << " endfacet\n";
            }

            template <typename T>
            static inline void save(ios::ostream &fp, const array< facet<T> > &facets )
            {
                fp << "solid\n";
                const size_t n = facets.size();
                for(size_t i=1;i<=n;++i)
                {
                    emit(fp, facets[i]);
                }
                fp << "endsolid\n";
            }
        };
    }
}

YOCTO_SUPPORT_C_STYLE_OPS(spade::stl::facet<float>);
YOCTO_SUPPORT_C_STYLE_OPS(spade::stl::facet<double>);


#endif
