#ifndef YOCTO_FOVEA_CELL_INCLUDED
#define YOCTO_FOVEA_CELL_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace fovea
    {
        
        
        class ICell : public object
        {
        public:
            virtual ~ICell() throw();
            const size_t vertices;
            const size_t edges;
            
            virtual size_t index_of( size_t iv ) const throw() = 0;
            
        protected:
            explicit ICell(size_t nv,size_t ne); // check #vertices, #edges
            void invalid_facet() const; // not a 3 or 4 vertices cell.
            void invalid_facet_edge() const; // can't find an edge !
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ICell);
        };
        
        template <size_t DIM,typename T>
        class Cell : public ICell
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef typename vertex_for<DIM,T>::type vtx;
            typedef Vertex<DIM,T>                    VERTEX;
            typedef Edge<DIM,T>                      EDGE;
            typedef Mesh<DIM,T>                      MESH;
            
            class List : public core::list_of<Cell>
            {
            public:
                explicit List() throw() : core::list_of<Cell>() { }
                virtual ~List() throw() { while(this->size) delete this->pop_back(); }
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(List);
            };
        
            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________
            //! access
            inline const VERTEX & operator[](size_t iv) const throw()
            {
                assert(iv<vertices);
                const VERTEX **p = ppVTX();
                assert(p);
                assert(p[iv]);
                return *p[iv];
            }
            
            virtual size_t index_of( size_t iv ) const throw()
            {
                assert(iv<vertices);
                const VERTEX **p = ppVTX();
                assert(p);
                assert(p[iv]);
                return p[iv]->index;
            }
            
            //! once physical vertices have been set
            virtual void compile( const MESH & ) = 0;
            
            virtual ~Cell() throw()
            {
            }
            
            
        protected:
            virtual const VERTEX **ppVTX() const throw() = 0;
            virtual const EDGE   **ppEDG() const throw() = 0;
            
            inline Cell( size_t nv, size_t ne) :
            ICell(nv,ne),
            bar()
            {
            }
            
            //__________________________________________________________________
            //
            // Computing Area(s)
            //__________________________________________________________________
            static inline void fill_sa( T &lhs, T &rhs, const vtx &p0, const vtx &p1 ) throw()
            {
                lhs += p0.x * p1.y;
                rhs += p1.x * p0.y;
            }
            
            
            inline int signed_area( T &a ) const throw()
            {
                assert(DIM==2);
                assert(vertices>=2);
                const VERTEX **p = ppVTX();
                const size_t top = vertices-1;
                T lhs = 0;
                T rhs = 0;
                for(size_t i=0,j=1;i<top;++i,++j)
                {
                    fill_sa(lhs,rhs,p[i]->r,p[j]->r);
                }
                fill_sa(lhs,rhs,p[top]->r,p[0]->r);
                a = 0;
                if(lhs>rhs)
                {
                    // Counter ClockWise
                    a = (lhs-rhs)/2;
                    return 1;
                }
                else
                {
                    if(lhs<rhs)
                    {
                        // ClockWise
                        a = (rhs-lhs)/2;
                        return -1;
                    }
                    else
                        return 0;
                }
            }
            
            //__________________________________________________________________
            //
            // edges for Triangles and Quads
            //__________________________________________________________________
            inline
            const EDGE *get_edge( const VERTEX &v1, const VERTEX &v2, const MESH &m ) const
            {
                const edge_key ek(v1.index,v2.index);
                const EDGE    *pE = m.edb.search(ek);
                if(!pE)
                    invalid_facet_edge();
                return pE;
            }
            
            inline void get_facet_edges(const MESH &m )
            {
                assert(this->vertices==3||this->vertices==4);
                assert(this->vertices==this->edges);
                const VERTEX **p = ppVTX();       assert(p);
                const EDGE   **e = this->ppEDG(); assert(e);
                switch(this->vertices)
                {
                    case 3:
                        e[0] = get_edge(*p[0], *p[1], m);
                        e[1] = get_edge(*p[1], *p[2], m);
                        e[2] = get_edge(*p[2], *p[0], m);
                        break;
                        
                    case 4:
                        e[0] = get_edge(*p[0], *p[1], m);
                        e[1] = get_edge(*p[1], *p[2], m);
                        e[2] = get_edge(*p[2], *p[3], m);
                        e[3] = get_edge(*p[3], *p[0], m);
                        break;
                        
                    default:
                        this->invalid_facet();
                }
            }
            
        public:
            const vtx bar; //!< barycenter
            inline void barycenter() throw()
            {
                const VERTEX **p = ppVTX();
                vtx &G = (vtx &)bar;
                bzset(G);
                for(size_t i=0;i<vertices;++i)
                {
                    G += p[i]->r;
                }
                T *g = (T*)&G;
                for(size_t i=0;i<DIM;++i)
                    g[i] /= vertices;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
            
        };
        
    }
}

#endif
