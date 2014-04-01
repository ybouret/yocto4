#ifndef YOCTO_FOVEA_SHAPE_TETRA_INCLUDED
#define YOCTO_FOVEA_SHAPE_TETRA_INCLUDED 1

#include "yocto/fovea/shape.hpp"

namespace yocto
{
    namespace fovea
    {
        struct TetraInfo
        {
            static const char *Name() throw();
            static void CheckVertices( const VertexBase &a, const VertexBase &b, const VertexBase &c, const VertexBase &d );
            static void NoEdgeFor( const VertexBase &a, const VertexBase &b );
            static void MissingEdge( const edge_key &ek );
            static void ZeroLength( const VertexBase &a, const VertexBase &b );
            
        };
        
        
        template <typename T>
        class Tetra : public Shape<3,T>
        {
        public:
            typedef Shape<3,T>                     SHAPE;
            typedef typename SHAPE::VERTEX         VERTEX;
            typedef typename SHAPE::EDGE           EDGE;
            typedef typename SHAPE::MESH           MESH;
            typedef typename VERTEX::vtx           vtx;
            
            virtual ~Tetra() throw() {}
            
            explicit Tetra(const VERTEX &a,
                           const VERTEX &b,
                           const VERTEX &c,
                           const VERTEX &d ) :
            SHAPE(4),
            p(),
            e()
            {
                TetraInfo::CheckVertices(a, b, c,d);
                p[0] = &a;
                p[1] = &b;
                p[2] = &c;
                p[3] = &d;
                e[0] = e[1] = e[2] = e[3] = e[4] = e[5] = 0;
            }
            
            inline virtual const char * name() const throw() { return TetraInfo::Name(); }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Tetra);
            const VERTEX *p[4];
            const EDGE   *e[6];
            
        };
        
        
    }
}

#endif

