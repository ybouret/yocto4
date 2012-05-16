#include "yocto/code/utils.hpp"

namespace yocto 
{
    namespace swamp
    {
        
        template <>
        delaunay<ZTYPE>:: triangle::~triangle() throw() {}
        
        
        
        ZTYPE __compute_radius_and_center(const delaunay<ZTYPE>::vertex &center,  
                                          const delaunay<ZTYPE>::vertex &A, 
                                          const delaunay<ZTYPE>::vertex &B,
                                          const delaunay<ZTYPE>::vertex &C)
        {
            delaunay<ZTYPE>::vertex &cc = (delaunay<ZTYPE>::vertex &)center;
            
            {
                const  delaunay<ZTYPE>::vertex AB(A,B);
                const  delaunay<ZTYPE>::vertex AC(A,C);
                const  delaunay<ZTYPE>::vertex BC(B,C);
                geom::m2d<ZTYPE> M_AB_AC;
                M_AB_AC.ex.x = AB.x; M_AB_AC.ey.x = AB.y;
                M_AB_AC.ex.y = AC.x; M_AB_AC.ey.y = AC.y;
                M_AB_AC.inverse();
                const delaunay<ZTYPE>::vertex U(ZTYPE(0.5) *( (A.x+B.x) * AB.x + (A.y+B.y) * AB.y ),
                                                ZTYPE(0.5) *( (A.x+C.x) * AC.x + (A.y+C.y) * AC.y ) );
                M_AB_AC.mul( cc, U );
            }
            const  delaunay<ZTYPE>::vertex MA(cc,A);
            const  delaunay<ZTYPE>::vertex MB(cc,B);
            const  delaunay<ZTYPE>::vertex MC(cc,C);
            const  ZTYPE ra = MA.norm();
            const  ZTYPE rb = MB.norm();
            const  ZTYPE rc = MC.norm();
            
            return (ra+rb+rc)/3;
        }
        
        template <>
        delaunay<ZTYPE>:: triangle::triangle( const array<vertex> &vertices, size_t a, size_t b, size_t c ) :
        iTriangle(a,b,c),
        center(),
        radius( __compute_radius_and_center(center, vertices[i0], vertices[i1], vertices[i2] ) ),
        r2( radius * radius )
        {
            
        }
        
        
        template <>
        delaunay<ZTYPE>:: triangle::triangle( const triangle &other ) throw() :
        iTriangle(other),
        next(0),
        prev(0),
        center( other.center ),
        radius( other.radius ),
        r2( other.r2 )
        {
        }
        
        
        template <>
        delaunay<ZTYPE>:: delaunay() : 
        tr_pool(), 
        tr_list(),
        circum()
        {}
        
        template <>
        void delaunay<ZTYPE>:: destruct( triangle *tr ) throw()
        {
            assert(tr);
            assert(tr->next == 0);
            assert(tr->prev == 0);
            tr->~triangle();
            tr_pool.store(tr);
        }
        
        
        template <>
        void delaunay<ZTYPE>:: free() throw()
        {
            while( tr_list.size ) destruct( tr_list.pop_back() );
        }
        
        template <>
        void delaunay<ZTYPE>:: kill() throw()
        {
            while( tr_pool.size ) object::release1<triangle>( tr_pool.query() );
        }
        
        
        
        template <>
        delaunay<ZTYPE>:: ~delaunay() throw() 
        {
            free();
            kill();
        }
        
        
        template <>
        delaunay<ZTYPE>::triangle * delaunay<ZTYPE>:: create( const array<vertex> &vertices, size_t a, size_t b, size_t c )
        {
            triangle *tr = tr_pool.size ? tr_pool.query() : object::acquire1<triangle>();
            try { new (tr) triangle(vertices,a,b,c); } catch(...) { tr_pool.store( tr ); throw; }
            return tr;
        }
        
        template <>
        const delaunay<ZTYPE>::triangles & delaunay<ZTYPE>::operator()(void) const throw()
        {
            return tr_list;
        }
        
        template <>
        void delaunay<ZTYPE>:: insert( const array<vertex> &vertices, size_t v_index )
        {
            assert(v_index>3);
            assert(v_index<=vertices.size());
            assert(tr_list.size>0);
            std::cerr << "insert #" << v_index << std::endl;
            circum.free();
            const vertex &v = vertices[v_index];
            triangle *inside = NULL;
            for( triangle *tr = tr_list.head; tr; tr = tr->next )
            {
                const vertex R(tr->center,v);
                if( R.norm2() <= tr->r2 )
                {
                    circum.push_back( tr );
                }
            }
            
            
            
        }
        
        
        template <>
        void delaunay<ZTYPE>:: build( const array<vertex> &vertices )
        {
            free();
            const size_t nv = vertices.size();
            if( nv < 3 )
                return;
            
            //------------------------------------------------------------------
            // push the first triangle
            //------------------------------------------------------------------
            tr_list.push_back( create(vertices,1,2,3) );
            
            //------------------------------------------------------------------
            // insert the other ones
            //------------------------------------------------------------------
            for( size_t i=4; 
                i <= min_of<size_t>(4,vertices.size()); 
                ++i )
                insert( vertices, i );
        }
        
        
        
        
    }
}