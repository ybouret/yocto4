#ifndef YOCTO_GFX_TYPES_INCLUDED
#define YOCTO_GFX_TYPES_INCLUDED 1

#include "yocto/math/point2d.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/parallel/patch.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace gfx
    {
        typedef point2d<unit_t>       vertex; //!< base class for operations
        typedef parallel::patch2D     patch;  //!< base class for patches
        typedef float                 real_t; //!< default floating point
        typedef math::complex<real_t> cplx_t; //!< complex for graphics...
        typedef point2d<real_t>       point;  //!< real point

#define YOCTO_GFX_R2GS 0.2126f
#define YOCTO_GFX_G2GS 0.7152f
#define YOCTO_GFX_B2GS 0.0722f

        struct gist
        {
            static const  float   unit_float[256];   //!< 0.0f..1.0f
            static const  float   unit_float_r[256]; //!< (0.0f..1.0f)*YOCTO_GFX_R2GS
            static const  float   unit_float_g[256]; //!< (0.0f..1.0f)*YOCTO_GFX_G2GS
            static const  float   unit_float_b[256]; //!< (0.0f..1.0f)*YOCTO_GFX_B2GS
            static inline uint8_t float2byte(const float x) throw() { return uint8_t(x*255.0f+0.5f); }
            static inline float   greyscalef(const uint8_t R,const uint8_t G,const uint8_t B) throw()
            {
                return unit_float_r[R] + unit_float_g[R] + unit_float_b[B];
            }

            static inline uint8_t greyscale1(const uint8_t R,const uint8_t G,const uint8_t B) throw()
            {
                return float2byte(greyscalef(R,G,B));
            }

            static const vertex delta[8]; //!< the 4+4 coordinates around a single point

            static inline unit_t float2unit(const float x) throw() { return unit_t(floorf(x+0.5f)); }
        };

        class vnode
        {
        public:
            vnode *next;
            vnode *prev;
            vertex vtx;
            YOCTO_MAKE_OBJECT
            inline  vnode() throw() : next(0), prev(0), vtx() {}
            inline ~vnode() throw() {}
            inline vnode(const vertex &v) throw() : next(0), prev(0), vtx(v) {}
            inline vnode(const unit_t x, const unit_t y) throw() : next(0), prev(0), vtx(x,y) {}
            inline vnode(const vnode &other) throw() : next(0), prev(0), vtx(other.vtx)  {}

        private:
            YOCTO_DISABLE_ASSIGN(vnode);
        };

        typedef core::list_of_cpp<vnode> _vlist;
        
        class vlist : public _vlist
        {
        public:
            explicit vlist() throw();
            virtual ~vlist() throw();
            vlist(const vlist &other);
            
            vertex width() const throw();
            vertex center() const throw();
            
            
        private:
            YOCTO_DISABLE_ASSIGN(vlist);
        };
        
        
    }
}

#endif

