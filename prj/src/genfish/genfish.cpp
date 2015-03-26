#include "yocto/math/types.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/fcn/zfind.hpp"
#include "yocto/math/v3d.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/math/dat/linear.hpp"
#include "yocto/math/dat/spline.hpp"

using namespace yocto;
using namespace math;

typedef numeric<double>::function Function;
typedef v3d<double>               vtx_t;

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
    vtx_t  n;

    Triangle(const pPoint &A,
             const pPoint &B,
             const pPoint &C) throw():
    a(A),b(B),c(C),
    n()
    {
        assert(a->i!=b->i);
        assert(a->i!=c->i);
        assert(b->i!=c->i);
        {
            const vtx_t G = (1.0/3) * (a->r+b->r+c->r);
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
            n.normalize();
        }
    }

    Triangle(const Triangle &other) throw() :
    a(other.a), b(other.b), c(other.c), n(other.n)
    {
    }

    inline ~Triangle() throw() {}


private:
    YOCTO_DISABLE_ASSIGN(Triangle);
};


class Fish
{
public:
    Function         Width;     //!< x axis
    Function         Up;        //!< y axis top radius (positive)
    Function         Down;      //!< y axis bottom radius (positive)
    Function         Mix;       //!< in [0:1] -> [0:1]
    const size_t     N;         //!< number of internal slices
    Function         Perimeter; //!<
    Function         Surface;   //!< cumulative surface
    vector<pSlice>   Slices;
    vector<pPoint>   Points;
    vector<Triangle> Triangles;

    derivative<double> drvs;
    double             drvs_h;
    integrator<double> intp;
    integrator<double> ints;
    const double       delta; //!< 1.0/(N+1)
    double             w;  //!< temporary width
    double             u;  //!< temporary top
    double             d;  //!< temporary bottom
    double             z0; //!< temporary z
    double             S0; //!< temporary surface


    explicit Fish(const Function &userW,
                  const Function &userU,
                  const Function &userD,
                  const Function &userM,
                  const size_t    userN) :
    Width(userW),
    Up(userU),
    Down(userD),
    Mix(userM),
    N(max_of<size_t>(userN,1)),
    Perimeter(this, & Fish:: __Perimeter),
    Surface(this,   & Fish:: __Surface  ),
    Slices(N+2,as_capacity),
    drvs(),
    drvs_h(1e-4),
    intp(),
    ints(),
    delta(1.0/(N+1)),
    XX0(this, & Fish:: __getX0),
    YY0(this, & Fish:: __getY0),
    dPerim0(this, & Fish:: __dPerimeter),
    TotalSurface( ComputeTotalSurface() )
    {
        std::cerr << "\t\tTotalSurface=" << TotalSurface << std::endl;

        pPoint p0( new Point() );
        Points.push_back(p0);

        //______________________________________________________________________
        //
        // pass 1: find the internal slices
        //______________________________________________________________________
        std::cerr << "\t*** Finding Internal Slices" << std::endl;

        /*
         Function zSurf(this, & Fish::__zSurf);
         zfind<double> solve(1e-4);
         double max_perimeter = 0;
         double last_z        = 0;
         for(size_t i=1;i<=N;++i)
         {
         std::cerr << "\t\tComputing Slice #" << i << "/" << N << std::endl;
         #if 1
         const double  z = i*delta;
         #else
         S0 = (i*TotalSurface)/(N+1);
         const double z  = solve(zSurf,last_z,1);
         #endif

         const double  p = Perimeter(z);
         pSlice        pS(new Slice(z,p) );
         Slices.push_back(pS);
         if(p>max_perimeter) max_perimeter = p;
         last_z = z;
         }

         */

        std::cerr << "\t*** Building Surface Approximation..." << std::endl;
        double max_perimeter = 0;
        {
            // First Pass: building approximate surface
            vector<double> zs(N+2,as_capacity);
            vector<double> vs(N+2,as_capacity);
            zs.push_back(0);
            vs.push_back(0);
            for(size_t i=1;i<=N;++i)
            {
                std::cerr << "\t\t\t" << i << "/" << N << "            \r"; std::cerr.flush();
                const double z = i*delta;
                const double s = vs.back() + ints(zs.back(),z,Perimeter,1e-4);
                zs.push_back(z);
                vs.push_back(s);
            }
            zs.push_back(1.0);
            vs.push_back(TotalSurface);
            {
                ios::ocstream fp("surface.dat",false);
                for(size_t i=1;i<=zs.size();++i)
                {
                    fp("%g %g\n", zs[i],vs[i]);
                }
            }

            std::cerr << std::endl;

            spline_function<double> spl(spline_tangent_both,zs,vs,0,0);
            {
                ios::ocstream fp("splines.dat",false);
                for(size_t i=0;i<=1000;++i)
                {
                    const double zz = i*0.001;
                    fp("%g %g\n", zz, spl.call(zz) );
                }
            }
            
            
            // Second Pass: approximated inversion
            vector<double> theZ(1,as_capacity);

            for(size_t i=1;i<=N;++i)
            {
                S0 = (i*TotalSurface)/(N+1);
                linear_find(S0, theZ, zs, vs,1);
                if(theZ.size()!=1)
                    throw exception("Cannot find S=%g",S0);
                const double z = theZ[1];
                const double p = Perimeter(z);
                pSlice       pS(new Slice(z,p) );
                Slices.push_back(pS);
                if(p>max_perimeter) max_perimeter = p;
            }

        }

        
        //______________________________________________________________________
        //
        // pass 2: compute how many points per slices M = 2+2*n
        //______________________________________________________________________
        std::cerr << "\t*** Finding Radial Slicing..." << std::endl;
        const size_t n = max_of<size_t>(2,ceil(max_perimeter/delta))-2;
        const size_t M = 2+2*n;
        std::cerr << "\t\tn=" << n << ", M=" << M << std::endl;

        std::cerr << "\t*** Computing Points..." << std::endl;
        for(size_t i=1;i<=N;++i)
        {
            Slice &slice = *Slices[i];

            for(size_t j=0;j<M;++j)
            {
                const double theta = (j*numeric<double>::two_pi)/M;
                pPoint pp( new Point() );
                pp->r = pos(theta,slice.z);
                slice.points.push_back(pp);
                Points.push_back(pp);
            }

        }

        pPoint pN( new Point() );
        Points.push_back(pN);
        pN->r.z = 1;

        std::cerr << "\t\t#Points=" << Points.size() << std::endl;

        //______________________________________________________________________
        //
        // pass 3: compute triangles
        //______________________________________________________________________
        std::cerr << "\t*** Computing Triangles" << std::endl;
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

        std::cerr << "\t\t#Triangles=" << Triangles.size() << std::endl;
        std::cerr << "\t\t#Points   =" << Points.size()    << std::endl;

        std::cerr << "\t*** Rescaling" << std::endl;
        for(size_t i=1; i <= Points.size();++i)
        {
            Point &p = *Points[i];
            p.r.z -= 0.5;
        }

    }

    double ComputeTotalSurface()
    {
        std::cerr << "\t*** Computing Total Surface" << std::endl;
        return Surface(1.0);
    }

    vtx_t pos(const double theta, const double z)
    {
        z0=z;
        return vtx_t(__getX0(theta),__getY0(theta),z);
    }

    static inline double g(double theta) throw()
    {
        return 0.5*(1+sin(theta));
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

#if 0
        const size_t NS = Slices.size();
        size_t NL = 0;
        for(size_t i=1;i<=NS;++i)
        {
            NL += Slices[i]->points.size();
        }

        fp("LINES %u %u\n", unsigned(NL), unsigned(3*NL) );
        for(size_t i=1;i<=NS;++i)
        {
            const array<pPoint> &points = Slices[i]->points;
            const size_t np=points.size();
            for(size_t j=1;j<=np;++j)
            {
                size_t jp = j+1;
                if(jp>np) jp = 1;
                fp("2 %u %u\n", unsigned(points[j]->i), unsigned(points[jp]->i) );
            }
        }
#endif

        const size_t NT = Triangles.size();

        fp("POLYGONS %u %u\n", unsigned(NT), unsigned(4*NT) );
        for(size_t i=1;i<=NT;++i)
        {
            const Triangle &tr = Triangles[i];
            fp("3 %u %u %u\n", unsigned(tr.a->i), unsigned(tr.b->i), unsigned(tr.c->i) );
        }

    }


    inline void __stl( ios::ostream &fp, const Point &p ) const
    {
        fp("   vertex %.6g %.6g %.6g\n", p.r.x, p.r.y,p.r.z);
    }

    void save_stl( const string &filename ) const
    {
        ios::ocstream fp(filename,false);

        fp("solid fish\n");

        const size_t NT = Triangles.size();
        for(size_t i=1;i<=NT;++i)
        {
            const Triangle &tr = Triangles[i];
            fp(" facet normal %.6g %.6g %.6g\n",tr.n.x,tr.n.y,tr.n.z);
            fp("  outer loop\n");
            __stl(fp, *tr.a);
            __stl(fp, *tr.b);
            __stl(fp, *tr.c);
            fp("  end loop\n");
            fp(" end facet\n");
        }
        fp("endsolid\n");
    }



private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Fish);
    Function XX0;
    Function YY0;
    Function dPerim0;

    double __getX0(double theta)
    {
        return Width(z0) * cos(theta);
    }

    double __getY0(double theta)
    {
        u = Up(z0);
        d = Down(z0);
        const double ratio = g(theta);
        const double m     = Mix(ratio);
        return (u*m+d*(1-m))*sin(theta);
    }

    double __dPerimeter(double theta)
    {
        const double dXdt = drvs(XX0,theta,drvs_h);
        const double dYdt = drvs(YY0,theta,drvs_h);
        return Hypotenuse(dXdt, dYdt);
    }

    double __Perimeter(double z)
    {
        if(z<=0||z>=1)
        {
            return 0;
        }
        else
        {
            z0=z;
            return intp(0,numeric<double>::two_pi, dPerim0, 1e-4);
        }
    }

    double __Surface(double z)
    {
        if(z<=0)
        {
            return 0;
        }
        else
        {
            if(z>=1)
            {
                z=1;
            }
            return ints(0,z,Perimeter,1e-4);
        }
    }

    double __zSurf(double z)
    {
        return S0 - __Surface(z);
    }

public:
    const double TotalSurface;


};

#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-maths.hpp"



int main(int argc, char *argv[] )
{
    static const char *program = vfs::get_base_name(argv[0]);
    try
    {

#if 1
        Lua::State VM;
        if(argc<=1)
        {
            throw exception("need a config.lua");
        }

        lua_State *L = VM();
        Lua::Config::DoFile(L,argv[1]);
        Lua::Function<double> Width(L,"Width",true);
        Lua::Function<double> Up(L,"Up",true);
        Lua::Function<double> Down(L,"Down",true);
        Lua::Function<double> Mix(L,"Mix",true);
        const size_t N = size_t(Lua::Config::Get<lua_Number>(L, "N"));
        Fish F(Width,Up,Down,Mix,N);
#else

        Function Width( cfunctor(WW) );
        Function Up(    cfunctor(UU) );
        Function Down(  cfunctor(DD) );
        Function Mix(   cfunctor(MM) );

        Fish F(Width,Up,Down,Mix,50);

        std::cerr << "TotalSurface=" << F.TotalSurface << std::endl;
#endif

        F.save_vtk("fish.vtk");
        F.save_stl("fish.stl");

        return 0;
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Uncaught exception in " << program << std::endl;
    }
    return 1;
}
