#ifndef YOCTO_POINT3D_INCLUDED
#define YOCTO_POINT3D_INCLUDED 1

#include "yocto/math/point2d.hpp"

namespace yocto
{

    template <typename T>
    class point3d
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        type x;
        type y;
        type z;

        //______________________________________________________________________
        //
        // POD behavior
        //______________________________________________________________________
        inline  point3d() : x(0), y(0), z(0) {}
        inline  point3d(param_type X, param_type Y, param_type Z) : x(X), y(Y), z(Z) {}
        inline ~point3d() throw() {}
        inline  point3d(const point3d &other) : x(other.x), y(other.y), z(other.z) {}
        inline  point3d(const point3d a, const point3d b) throw() : x(b.x-a.x), y(b.y-a.y), z(b.z-a.z) {}
        inline  point3d & operator=(const point3d &other)
        {
            if(this!=&other)
            {
                x = other.x;
                y = other.y;
                z = other.z;
            }
            return *this;
        }

        inline friend std::ostream & operator<<( std::ostream &os, const point3d &p )
        {
            os << '[' << p.x << ' ' << p.y << ' ' << p.z << ']' << '\'';
            return os;
        }


        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________
        inline friend point3d   operator+ (const point3d &p) { return p; }
        inline friend point3d   operator+ (const point3d &lhs,const point3d &rhs) { return point3d(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z); }
        inline        point3d & operator+=(const point3d &rhs) { x+=rhs.x; y+=rhs.y; z+=rhs.z; return *this;}

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________
        inline friend point3d   operator- (const point3d &p) { return point3d(-p.x,-p.y,-p.z); }
        inline friend point3d   operator- (const point3d &lhs,const point3d &rhs) { return point3d(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z); }
        inline        point3d & operator-=( const point3d &rhs ) { x-=rhs.x; y-=rhs.y; z-=rhs.z; return *this;}

        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________
        inline friend point3d operator*(param_type a,const point3d &p) { return point3d(a*p.x,a*p.y,a*p.z); }
        inline point3d & operator*=(param_type a) { x*=a; y*=a; z*=a; return *this; }

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        inline friend point3d operator/(const point3d &p,param_type a) { return point3d(p.x/a,p.y/a,p.z/a); }
        inline point3d & operator/=(param_type a) { x/=a; y/=a; z/=a; return *this; }

        //______________________________________________________________________
        //
        // vectorial
        //______________________________________________________________________
        inline friend const_type operator*(const point3d &lhs, const point3d &rhs) throw()
        {
            return (lhs.x*rhs.x) + (lhs.y*rhs.y) + (lhs.z*rhs.z);
        }

        //______________________________________________________________________
        //
        // cross product
        //______________________________________________________________________
        inline friend point3d operator^(const point3d &a, const point3d &b) throw()
        {
            return point3d(a.y*b.z-a.z*b.y,b.x*a.z-a.x*b.z,a.x*b.y-a.y*b.x);
        }
        


    };
    
    
}

YOCTO_SUPPORT_C_STYLE_OPS(point3d<float>);
YOCTO_SUPPORT_C_STYLE_OPS(point3d<double>);
YOCTO_SUPPORT_C_STYLE_OPS(point3d<unit_t>);

#endif

