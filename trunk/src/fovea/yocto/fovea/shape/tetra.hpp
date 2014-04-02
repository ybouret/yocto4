#ifndef YOCTO_FOVEA_SHAPE_TETRA_INCLUDED
#define YOCTO_FOVEA_SHAPE_TETRA_INCLUDED 1

#include "yocto/fovea/shape/triangle.hpp"

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
            typedef Triangle<3,T>                  FACET;
            
            virtual ~Tetra() throw() {}
            
            explicit Tetra(const VERTEX &a,
                           const VERTEX &b,
                           const VERTEX &c,
                           const VERTEX &d) :
            SHAPE(4),
            p(),
            e(),
            f0(a,b,c),
            f1(a,b,d),
            f2(a,c,d),
            f3(b,c,d),
            f()
            {
                TetraInfo::CheckVertices(a, b, c,d);
                p[0] = &a;
                p[1] = &b;
                p[2] = &c;
                p[3] = &d;
                e[0] = e[1] = e[2] = e[3] = e[4] = e[5] = 0;
                f[0] = &f0;
                f[1] = &f1;
                f[2] = &f2;
                f[3] = &f3;
            }
            
            inline virtual const char * name() const throw() { return TetraInfo::Name(); }
            
            virtual void compile_for( const MESH &m )
            {
                //______________________________________________________________
                //
                // get all the edges
                //______________________________________________________________
                size_t k=0;
                for(size_t i=0;i<3;++i)
                {
                    const VERTEX &v1 = *p[i];
                    for(size_t j=i+1;j<4;++j)
                    {
                        const VERTEX &v2 = *p[j];
                        const edge_key ek(v1.index,v2.index);
                        assert(k<6);
                        e[k] = m.edges.search(ek);
                        if( !e[k] )
                            TetraInfo:: MissingEdge(ek);
                        ++k;
                    }
                }
                
                //______________________________________________________________
                //
                // get all the facets
                //______________________________________________________________
                this->compute_barycenter();
                for(size_t i=0;i<3;++i)
                {
                    FACET &ff = *f[i];
                    ff.compile_for(m);
                    const vtx GF(this->G,ff.G);
                    if( GF*ff.sn < 0 ) ff.flip_normal();
                }
                
                
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Tetra);
            const VERTEX *p[4];
            const EDGE   *e[6];
            FACET         f0,f1,f2,f3;
            FACET        *f[4];
            
            virtual const VERTEX **ppVTX() const throw() { return (const VERTEX **)p; }
            
        };
        
        
    }
}

#endif

