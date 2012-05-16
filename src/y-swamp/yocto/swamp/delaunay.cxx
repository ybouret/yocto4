

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
        
    }
}