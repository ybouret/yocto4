#ifndef YOCTO_FOVEA_VERTEX_INCLUDED
#define YOCTO_FOVEA_VERTEX_INCLUDED 1

#include "yocto/fovea/arrays.hpp"

namespace yocto
{
    namespace fovea
    {
        
        template <typename T>
        class Vertex
        {
        public:
#if 0
            class Handle
            {
            public:
                Handle *next;
                Handle *prev;
                Vertex *addr;
                YOCTO_MAKE_OBJECT
                inline  Handle( Vertex &v ) throw() : next(0), prev(0), addr(&v) {}
                inline ~Handle() throw() {}
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Handle);
            };
#endif
            
            explicit Vertex(T &cx) throw() :
            r()
            {
                r[0] = &cx;
                r[1] = r[2] = 0;
            }
            
            explicit Vertex(T &cx, T &cy) throw() :
            r()
            {
                r[0] = &cx;
                r[1] = &cy;
                r[2] = 0;
            }
            
            explicit Vertex(T &cx, T &cy, T &cz) throw() :
            r()
            {
                r[0] = &cx;
                r[1] = &cy;
                r[2] = &cz;
            }
            
            inline ~Vertex() throw() {}
            
            inline T       &x() throw()       { assert(r[0]!=0); return *r[0]; }
            inline const T &x() const throw() { assert(r[0]!=0); return *r[0]; }
            
            inline T       &y() throw()       { assert(r[1]!=0); return *r[1]; }
            inline const T &y() const throw() { assert(r[1]!=0); return *r[1]; }
            
            inline T       &z() throw()       { assert(r[2]!=0); return *r[2]; }
            inline const T &z() const throw() { assert(r[2]!=0); return *r[2]; }
            
        private:
            T *r[3];
            YOCTO_DISABLE_ASSIGN(Vertex);
            
        public:
            //core::list_of<Handle> neighbors;
        };
        
    }
}

#endif

