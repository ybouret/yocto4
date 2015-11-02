#ifndef YOCTO_GRAPHIX_TYPES_INCLUDED
#define YOCTO_GRAPHIX_TYPES_INCLUDED 1

#include "yocto/parallel/splitter.hpp"
#include "yocto/core/list.hpp"

#include <cmath>

namespace yocto
{
    namespace graphics
    {
        typedef point2d<unit_t>   vertex; //!< base class for operations
        typedef parallel::patch2D patch;  //!< base class for patches

        struct gist
        {
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


        };

        class vnode
        {
        public:
            vnode *next;
            vnode *prev;
            vertex v;
            inline  vnode() throw() : next(0), prev(0), v() {}
            inline  vnode(const vertex u) throw() : next(0), prev(0), v(u) {}
            inline ~vnode() throw() {}
            inline  vnode(const vnode &other) throw() : next(0), prev(0), v(other.v) {}

            YOCTO_MAKE_OBJECT
        private:
            YOCTO_DISABLE_ASSIGN(vnode);
        };

        typedef core::list_of_cpp<vnode> _vlist;

        class vlist : public _vlist
        {
        public:
            vlist *next;
            vlist *prev;

            inline explicit vlist() throw() : _vlist(), next(0), prev(0) {}
            inline virtual ~vlist() throw() {}
            inline vlist(const vlist &other) : _vlist(other), next(0), prev(0) {}

        private:
            YOCTO_DISABLE_ASSIGN(vlist);
        };
        
        typedef core::list_of_cpp<vlist> vlists;
        
        
    }
}

#endif
