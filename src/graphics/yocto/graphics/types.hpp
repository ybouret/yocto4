#ifndef YOCTO_GRAPHIX_TYPES_INCLUDED
#define YOCTO_GRAPHIX_TYPES_INCLUDED 1

#include "yocto/parallel/splitter.hpp"
#include "yocto/core/list.hpp"
#include "yocto/math/complex.hpp"
#include <cmath>

namespace yocto
{
    namespace graphics
    {
        typedef point2d<unit_t>       vertex; //!< base class for operations
        typedef parallel::patch2D     patch;  //!< base class for patches
        typedef float                 real_t; //!< default floating point
        typedef math::complex<real_t> cplx_t; //!< complex for graphics...

        class vnode_type
        {
        public:
            explicit vnode_type(const vertex &v) throw() : next(0), prev(0), vtx(v) {}
            vnode_type(const vnode_type &node) throw() :   next(0), prev(0), vtx(node.vtx) {}
            virtual ~vnode_type() throw() {}
            
            vnode_type *next;
            vnode_type *prev;
            vertex      vtx;
            
        private:
            YOCTO_DISABLE_ASSIGN(vnode_type);
        };
        
        typedef core::list_of_cpp<vnode_type> vnode_list;

        struct gist
        {
            static inline int fast_compare( const vertex &lhs, const vertex &rhs ) throw()
            {
                if(lhs.x==rhs.x&&lhs.y==rhs.y) return 0; else return -1;
            }

            static inline bool are_touching(const vertex &lhs, const vertex &rhs) throw()
            {
                const vertex d = lhs-rhs;
                return (d.x>=-1) && (d.x<=1) && (d.y>=-1) && (d.y<=1);
            }

            static const  float   unit_float[256];
            static inline uint8_t float2byte(const float x) throw() { return uint8_t(floorf(255.0f*x+0.5f)); }

            static inline float   greyscalef(const uint8_t R,const uint8_t G,const uint8_t B) throw()
            {
                const float r = 0.2126f*unit_float[R];
                const float g = 0.7152f*unit_float[G];
                const float b = 0.0722f*unit_float[B];
                const float Y = r + g + b;
                return Y;
            }

            static inline uint8_t greyscale1(const uint8_t R,const uint8_t G,const uint8_t B) throw()
            {
                return float2byte(greyscalef(R,G,B));
            }

            template <typename T>
            static T float_to(const float x) throw();


            //! the 4+4 coordinates around a single point
            static const vertex delta[8];

            //! way to fill when on border
            enum filling
            {
                fill_with_zero,
                fill_with_same,
                fill_with_symm,
                fill_with_asym,
                fill_no_fluxes
            };

            template <typename T,typename U> static inline
            T get_fill(const U      &f0,
                       const U      &f1,
                       const U      &f2,
                       const filling fill) throw()
            {
                switch(fill)
                {
                    case fill_with_same:
                        return T(f0);

                    case fill_with_symm:
                        return T(f1);

                    case fill_with_asym:
                        return T(f0)+T(f0)-T(f1);

                    case fill_no_fluxes:
                        return T(3) * ( T(f0) - T(f1) ) + T(f2);
                        
                    case fill_with_zero:
                        break;
                }
                assert(fill_with_zero==fill);
                return T(0);
            }

        };

        template <>
        inline float gist:: float_to<float>(const float x) throw()
        {
            return x;
        }

        template <>
        inline uint8_t gist:: float_to<uint8_t>(const float x) throw()
        {
            return float2byte(x);
        }



    }
}

#endif
