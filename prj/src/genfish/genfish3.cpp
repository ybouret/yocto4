#include "yocto/program.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-maths.hpp"
#include "yocto/math/dat/b-splines.hpp"
#include "yocto/math/v3d.hpp"
#include "yocto/math/fcn/zfind.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/code/utils.hpp"

using namespace yocto;
using namespace math;

typedef CubicApproximation<double,v3d> CubiX;
typedef CubiX::vtx_t vtx_t;

class Point : public counted_object
{
public:
    const size_t i;
    vtx_t        r;
    inline  Point() : i(Index++), r() {}
    virtual ~Point() throw() {}

private:
    static size_t Index;
};

size_t Point::Index = 0;

typedef arc_ptr<Point> pPoint;


class Slice : public counted_object
{
public:
    const double   z;
    const double   perimeter;
    vector<pPoint> points;
    explicit Slice(double zz,double pr) :
    z(zz),
    perimeter(pr),
    points()
    {
    }

    virtual ~Slice() throw() {}

private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Slice);
};

typedef arc_ptr<Slice> pSlice;

class Triangle
{
public:
    pPoint a,b,c;
    vtx_t  G;
    vtx_t  n;
    double S;
    Triangle(const pPoint &A,
             const pPoint &B,
             const pPoint &C) throw():
    a(A),b(B),c(C),
    G( (1.0/3) * (a->r+b->r+c->r) ),
    n(),
    S(0)
    {
        assert(a->i!=b->i);
        assert(a->i!=c->i);
        assert(b->i!=c->i);
        {
            //const vtx_t G = (1.0/3) * (a->r+b->r+c->r);
            const vtx_t AB(a->r,b->r);
            const vtx_t AC(a->r,c->r);
            const vtx_t NN = vtx_t::cross_(AB, AC);
            const vtx_t middle(0,0,0.5);
            const vtx_t Q(middle,G);
            if( (NN*Q) <= 0 )
            {
                b.swap_with(c);
            }
        }

        {
            const vtx_t AB(a->r,b->r);
            const vtx_t AC(a->r,c->r);
            n = vtx_t::cross_(AB, AC);
            S = n.norm();
            n.normalize();
        }
    }

    Triangle(const Triangle &other) throw() :
    a(other.a), b(other.b), c(other.c),
    G(other.G),
    n(other.n),
    S(other.S)
    {
    }

    inline ~Triangle() throw() {}

    void recompute() throw()
    {
        G = (1.0/3) * (a->r+b->r+c->r);
        const vtx_t AB(a->r,b->r);
        const vtx_t AC(a->r,c->r);
        n = vtx_t::cross_(AB, AC);
        S = n.norm();
        n.normalize();
    }

private:
    YOCTO_DISABLE_ASSIGN(Triangle);
};



static inline
void LoadCoordinates( sequence<vtx_t> &beta, lua_State *L, const string &name)
{
    lua_settop(L, 0);
    lua_getglobal(L,name.c_str());
    if( !lua_istable(L, -1) )
        throw exception("No Table '%s'", name.c_str());

    const size_t n = lua_rawlen(L, -1);
    if(n<1)
        throw exception("Need At Least 1 point in '%s'", name.c_str());

    beta.free();
    beta.ensure(n+2);

    {
        vtx_t tmp(0,0,0);
        beta.push_back(tmp);
    }
    for(size_t i=1;i<=n;++i)
    {
        lua_rawgeti(L, -1, i);
        if(!lua_isnumber(L, -1))
        {
            throw exception("%s[%u] is not a number", name.c_str(), unsigned(i));
        }
        const double t = lua_tonumber(L, -1);
        lua_pop(L,1);
        vtx_t v(t,t,0);

        if(i<=1)
        {
            v.z=0;
        }
        else
        {
            if(i>=n)
            {
                v.z = 1;
            }
            else
            {
                v.z = double(i-1)/(n-1);
            }
        }
        beta.push_back(v);
    }

    {
        vtx_t tmp(0,0,1);
        beta.push_back(tmp);
    }


}


class Fish
{
public:
    CubiX        W;
    CubiX        H;
    const size_t N;
    const double Length;
    vector<pSlice>   Slices;
    vector<pPoint>   Points;
    vector<Triangle> Triangles;


    explicit Fish(lua_State *L) :
    W(),
    H(),
    N( Lua::Config::Get<lua_Number>(L,"N")),
    Length( Lua::Config::Get<lua_Number>(L,"Length") )
    {
        LoadCoordinates(W, L, "width");
        LoadCoordinates(H, L, "height");
        {
            ios::ocstream fp("profiles.dat",false);
            for(double u=0;u<=1.0;u+=0.01)
            {
                const vtx_t w = W.Compute(u);
                const vtx_t h = H.Compute(u);
                fp("%g %g %g %g\n",w.z,w.x,h.z,h.y);
            }
        }

        {
            ios::ocstream fp("beta_h.dat",false);
            for(size_t i=1;i<=H.size();++i)
            {
                fp("%g %g\n", H[i].z, H[i].y);
            }
        }

        {
            ios::ocstream fp("beta_w.dat",false);
            for(size_t i=1;i<=W.size();++i)
            {
                fp("%g %g\n", W[i].z, W[i].x);
            }
        }

        //______________________________________________________________________
        //
        // pass 1: compute profiles position
        //______________________________________________________________________

        zfunction<double> zW( &W, & CubiX::Z, 0);
        zfunction<double> zH( &H, & CubiX::Z, 0);
        zfind<double>     solve(0);

        double max_perimeter = 0;
        {
            ios::ocstream fp("slices.dat",false);
            for(size_t i=1;i<=N;++i)
            {
                const double z = double(i)/(N+1);
                zW.target = zH.target = z;
                const double uW = solve(zW.call,0,1);
                const double uH = solve(zH.call,0,1);

                const double width  = W.X(uW);
                const double height = H.Y(uH);

                const double p = numeric<double>::pi * sqrt( 2.0 * (width*width+height*height) );
                fp("%g %g %g %g\n",z,width,height,p);
                if(p>max_perimeter) max_perimeter = p;
                pSlice       pS(new Slice(z,p) );
                Slices.push_back(pS);
            }
        }
        std::cerr << "-- Max Perimeter=" << max_perimeter << std::endl;


        //______________________________________________________________________
        //
        // pass 2: compute how many points per slices M = 2+2*n
        //______________________________________________________________________
        std::cerr << "-- Finding Radial Slicing..." << std::endl;
        const double delta = 1.0/(N+1);
        const size_t n = max_of<size_t>(2,ceil(max_perimeter/delta))-2;
        const size_t M = 2+2*n;
        std::cerr << "\tn=" << n << ", M=" << M << std::endl;

        std::cerr << "-- Computing Points..." << std::endl;

        pPoint p0( new Point() );
        Points.push_back(p0);

        for(size_t i=1;i<=N;++i)
        {
            Slice &slice = *Slices[i];
            zW.target = zH.target = slice.z;

            const double uW = solve(zW.call,0,1);
            const double uH = solve(zH.call,0,1);

            const double width  = W.X(uW);
            const double height = H.Y(uH);

            //std::cerr << "w=" << width << ", h=" << height << std::endl;
            for(size_t j=0;j<M;++j)
            {
                const double theta = (j*numeric<double>::two_pi)/M;
                pPoint pp( new Point() );

                pp->r.x = width  * cos(theta);
                pp->r.y = height * sin(theta);
                pp->r.z = slice.z;

                slice.points.push_back(pp);
                Points.push_back(pp);
            }

        }



        pPoint pN( new Point() );
        Points.push_back(pN);
        pN->r.z = 1;

        std::cerr << "\t#Points=" << Points.size() << std::endl;
        
        //______________________________________________________________________
        //
        // pass 3: compute triangles
        //______________________________________________________________________
        std::cerr << "-- Computing Triangles" << std::endl;
        std::cerr << "\t Head..." << std::endl;
        // head
        {
            const Slice &slice = *Slices[1];
            for(size_t i=1;i<=M;++i)
            {
                size_t   ip = i+1;
                if(ip>M) ip = 1;
                const Triangle tr(p0,slice.points[i],slice.points[ip]);
                Triangles.push_back(tr);
            }
        }

        std::cerr << "\t Body..." << std::endl;
        // inside
        for(size_t j=1;j<N;++j)
        {
            const array<pPoint> &P0 = Slices[j]->points;
            const array<pPoint> &P1 = Slices[j+1]->points;

            // loop over quads
            for(size_t i=1;i<=M;++i)
            {
                size_t   ip = i+1;
                if(ip>M) ip = 1;
                const pPoint  &P00 = P0[i];
                const pPoint  &P01 = P0[ip];
                const pPoint  &P10 = P1[i];
                const pPoint  &P11 = P1[ip];

                {
                    const Triangle tr(P00,P01,P11);
                    Triangles.push_back(tr);
                }

                {
                    const Triangle tr(P00,P10,P11);
                    Triangles.push_back(tr);
                }


            }
        }

        std::cerr << "\t Tail..." << std::endl;
        // tail
        {
            const Slice &slice = *Slices[N];
            for(size_t i=1;i<=M;++i)
            {
                size_t   ip = i+1;
                if(ip>M) ip = 1;
                const Triangle tr(pN,slice.points[i],slice.points[ip]);
                Triangles.push_back(tr);
            }
        }

        //______________________________________________________________________
        //
        // pass 4: rescale
        //______________________________________________________________________

        std::cerr << "\t#Triangles=" << Triangles.size() << std::endl;
        std::cerr << "\t#Points   =" << Points.size()    << std::endl;

        std::cerr << "-- Rescaling" << std::endl;
        const size_t NP = Points.size();
        for(size_t i=1; i <= NP;++i)
        {
            Point &p = *Points[i];
            p.r.z -= 0.5;
            p.r.x *= Length;
            p.r.y *= Length;
            p.r.z *= Length;

        }

        //______________________________________________________________________
        //
        // pass 5: Other Geometry
        //______________________________________________________________________
        vtx_t G;
        for(size_t i=1;i<=NP;++i)
        {
            G += Points[i]->r;
        }
        G *= 1.0/NP;
        std::cerr << "G=" << G << std::endl;

        double volume  = 0 ;
        double surface = 0;
        for(size_t i=Triangles.size();i>0;--i)
        {
            Triangle &tr = Triangles[i];
            tr.recompute();
            surface += tr.S;
            volume  += (tr.G*tr.n) * tr.S;
        }
        volume /= 3;
        std::cerr << "Approx Surface: " << surface << std::endl;
        std::cerr << "Approx Volume : " << volume  << std::endl;


    }

    virtual ~Fish() throw()
    {
    }



    void save_vtk( const string &filename ) const
    {
        const size_t NP = Points.size();
        ios::ocstream fp(filename,false);
        fp << "# vtk DataFile Version 3.0\n";
        fp << "Fish\n";
        fp << "ASCII\n";
        fp << "DATASET POLYDATA\n";
        fp("POINTS %u float\n", unsigned(NP));
        for(size_t i=1;i<=NP;++i)
        {
            const vtx_t &r = Points[i]->r;
            fp("%g %g %g\n",r.x,r.y,r.z);
        }

        const size_t NT = Triangles.size();

        fp("POLYGONS %u %u\n", unsigned(NT), unsigned(4*NT) );
        for(size_t i=1;i<=NT;++i)
        {
            const Triangle &tr = Triangles[i];
            fp("3 %u %u %u\n", unsigned(tr.a->i), unsigned(tr.b->i), unsigned(tr.c->i) );
        }
        
    }


private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Fish);
};


YOCTO_PROGRAM_START()
{
    Lua::State VM;
    if(argc<=1)
    {
        throw exception("need a config.lua");
    }
    
    lua_State *L = VM();
    Lua::Config::DoFile(L,argv[1]);
    
    Fish fish(L);
    
    fish.save_vtk("fish.vtk");
    
    
    
}
YOCTO_PROGRAM_END()
