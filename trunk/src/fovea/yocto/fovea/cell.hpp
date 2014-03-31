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
            virtual size_t index_of( size_t iv ) const throw() = 0;
            
        protected:
            explicit ICell(size_t nv); // check #vertices
            
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
                assert(p);
                assert(p[iv]);
                return *p[iv];
            }
            
            virtual size_t index_of( size_t iv ) const throw()
            {
                assert(iv<vertices);
                assert(p);
                assert(p[iv]);
                return p[iv]->index;
            }
            
            //! once physical vertices have been set
            virtual void compile( const MESH & ) = 0;
            
            virtual ~Cell() throw()
            {
                object::operator delete(p,vertices * sizeof(VERTEX*));
            }
            
            
        protected:
            const VERTEX **p;
            
            inline Cell( size_t nv ) :
            ICell(nv),
            p(  (const VERTEX **)(object::operator new(vertices * sizeof(VERTEX*) )) ),
            bar()
            {
            }
            
            static inline void fill_sa( T &lhs, T &rhs, const vtx &p0, const vtx &p1 ) throw()
            {
                lhs += p0.x * p1.y;
                rhs += p1.x * p0.y;
            }
            
            inline int signed_area( T &a ) const throw()
            {
                assert(DIM==2);
                assert(vertices>=2);
                const size_t top = vertices-1;
                T lhs = 0;
                T rhs = 0;
                for(size_t i=0,j=1;i<top;++i,++j)
                {
                    fill_sa(lhs,rhs,this->p[i]->r,this->p[j]->r);
                }
                fill_sa(lhs,rhs,this->p[top]->r,this->p[0]->r);
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
            
        public:
            const vtx bar; //!< barycenter
            inline void barycenter() throw()
            {
                vtx &G = (vtx &)bar;
                bzset(G);
                for(size_t i=0;i<vertices;++i)
                {
                    G += this->p[i]->r;
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
