#include "yocto/math/alg/delaunay.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/core/list.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace
        {
            
            struct iEdge
            {
                size_t p1, p2;
                iEdge *next,*prev;
                
                
                
                inline
                static iEdge *Create(size_t a,size_t b)
                {
                    iEdge *edge = object::acquire1<iEdge>();
                    edge->p1 = a;
                    edge->p2 = b;
                    return edge;
                }
                
                inline
                static void Delete( iEdge *edge ) throw()
                {
                    assert(edge);
                    object::release1(edge);
                }
                
                class List : public core::list_of<iEdge>
                {
                public:
                    explicit List() throw() {}
                    virtual ~List() throw() { delete_with( iEdge::Delete ); }
                    
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(List);
                };
            };
            
            static inline
            void __insert_edge( core::list_of<iEdge> &edges, const size_t p1, const size_t p2)
            {
                for( iEdge *edge = edges.head;edge;edge=edge->next)
                {
                    if( (p1 == edge->p1 && p2 == edge->p2) || (p1 == edge->p2 && p2 == edge->p1) )
                    {
                        iEdge::Delete( edges.unlink(edge) );
                        return;
                    }
                }
                edges.push_back( iEdge::Create(p1, p2) );
            }
            
        }
        
        void delaunay_hull(sequence<size_t>       &h,
                           const array<iTriangle> &trlist)
        {
            
            h.free();
            
            //==================================================================
            // Create list of unique edges
            //==================================================================
            iEdge::List edges;
            
            for( size_t i=1; i <= trlist.size(); ++i )
            {
                const iTriangle &tr = trlist[i];
                __insert_edge(edges, tr.p1, tr.p2);
                __insert_edge(edges, tr.p2, tr.p3);
                __insert_edge(edges, tr.p3, tr.p1);
            }
            //std::cerr << "found " << edges.size << " edges on hull for " << trlist.size() << " triangles" << std::endl;
            h.reserve( edges.size + 1);
            //==================================================================
            // make the hull indices
            //==================================================================
            if( edges.size )
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                iEdge *edge = edges.pop_front();
                h.push_back(edge->p1);
                h.push_back(edge->p2);
                iEdge::Delete(edge);
                
                //--------------------------------------------------------------
                // connect other
                //--------------------------------------------------------------
            FIND_NEXT:
                if(edges.size<=0)
                {
                    h.pop_back();
                    return;
                }
                for(edge=edges.head;edge;edge=edge->next)
                {
                    if( edge->p1 == h.back() )
                    {
                        h.push_back(edge->p2);
                        iEdge::Delete( edges.unlink(edge) );
                        goto FIND_NEXT;
                    }
                    
                    if( edge->p2 == h.back() )
                    {
                        h.push_back(edge->p1);
                        iEdge::Delete( edges.unlink(edge) );
                        goto FIND_NEXT;
                    }
                }
                throw exception("delaunay_hull: invalid triangulation");
                
                
            }
        }
        
    }
}
