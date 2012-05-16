

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
            cc.x = cc.y = 0;
            const ZTYPE ByCy = B.y - C.y;
            const ZTYPE CyAy = C.y - A.y;
            const ZTYPE AyBy = A.y - B.y;
            const ZTYPE tmp  = A.x * ByCy + B.x * CyAy + C.x * AyBy;
            const ZTYPE D    = tmp+tmp;
            const ZTYPE A2   = A.norm2();
            const ZTYPE B2   = B.norm2();
            const ZTYPE C2   = C.norm2();
            
            cc.x = (A2 * ByCy + B2 * CyAy + C2 * AyBy)/D;
            cc.y = (A2 * (C.x - B.x) + B2 * (A.x - C.x ) + C2 * (B.x - A.x ) ) / D;
            
            const ZTYPE a = Sqrt(A2);
            const ZTYPE b = Sqrt(B2);
            const ZTYPE c = Sqrt(C2);
            const ZTYPE s = (a+b+c)/2;
            const ZTYPE den = s*(s-a)*(s-b)*(s-c);
            const ZTYPE num = a*b*c;
            return num/(4*Sqrt(den));
        }
        
        template <>
        delaunay<ZTYPE>:: triangle::triangle( const array<vertex> &vertices, size_t a, size_t b, size_t c ) :
        iTriangle(a,b,c),
        center(),
        radius( __compute_radius_and_center(center, vertices[i0], vertices[i1], vertices[i2] ) )
        {
            
        }
        
        
        template <>
        delaunay<ZTYPE>:: triangle::triangle( const triangle &other ) throw() :
        iTriangle(other),
        next(0),
        prev(0),
        center( other.center ),
        radius( other.radius )
        {
        }
        
        
        template <>
        delaunay<ZTYPE>:: delaunay() : tr_pool(), tr_list() {}
        
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
        }
        
        
        
        
    }
}