#include "yocto/math/dat/spline.hpp"
#include "yocto/program.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-maths.hpp"
#include "yocto/math/v3d.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/math/fcn/zfind.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"

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




#define FTOL 1e-4

class Fish
{
public:
    Function         Width;
    Function         Up;
    Function         Down;
    Function         Mix;
    Function         dLength;
    Function         X;          //!< for ztmp
    Function         Y;          //!< for ztmp
    Function         dPerimeter; //!< for ztmp
    Function         Perimeter;  //!< function of z
    const size_t     N;          //!< internal slices
    vector<pSlice>   Slices;
    vector<pPoint>   Points;
    vector<Triangle> Triangles;

    inline vtx_t gen_vtx(const double theta, const double z)
    {
        ztmp=z;
        return vtx_t(X(theta),Y(theta),z);
    }

    inline Fish(const Function &userWidth,
                const Function &userUp,
                const Function &userDown,
                const Function &userMix,
                const size_t   userN
                ) :
    Width( userWidth ),
    Up(userUp),
    Down(userDown),
    Mix(userMix),
    dLength( this, & Fish:: __dLength),
    X( this, & Fish:: __X ),
    Y( this, & Fish:: __Y ),
    dPerimeter(this, & Fish::__dPerimeter),
    Perimeter(this,  & Fish::__Perimeter),
    N( max_of<size_t>(1,userN) ),
    Slices(N+2,as_capacity),
    Points(),
    Triangles(),
    drvs(),
    drvs_h(1e-4),
    ztmp(0),
    intp()
    {

        //______________________________________________________________________
        //
        // pass 1: slicing
        //______________________________________________________________________

#define NZ 1000

        vector<double> pos(NZ+1,as_capacity);
        vector<double> arc(NZ+1,as_capacity);

        pos.push_back(0);
        arc.push_back( Width(pos.back()) );
        for(size_t i=1;i<=NZ;++i)
        {
            const double z_new = double(i)/NZ;
            const double w_new = Width(z_new);
            const double z_old = pos.back();
            const double w_old = Width(z_old);
            const double a_new = arc.back() + Hypotenuse(z_new-z_old, w_new-w_old);
            pos.push_back(z_new);
            arc.push_back(a_new);
        }

        const double arc_length = arc.back();
        std::cerr << "-- Arc Length=" << arc_length << std::endl;
        {
            ios::ocstream fp("arc.dat",false);
            for(size_t i=1;i<=NZ+1;++i)
            {
                fp("%g %g\n", pos[i], arc[i]);
            }
        }

        spline1D<double> spl(spline_natural, pos, arc);
        {
            ios::ocstream fp("spl.dat",false);
            const size_t NMAX = NZ*4;
            for(size_t i=0;i<=NMAX;++i)
            {
                const double zz = double(i)/NMAX;
                fp("%g %g\n",zz,spl(zz));
            }
        }

        zfunction<double> zfn( &spl, & spline1D<double>::get, 0);
        zfind<double>     solve(0);
        double max_perimeter = 0;
        for(size_t i=1;i<=N;++i)
        {
            zfn.target = (arc_length * i)/(N+1);
            const double zs = solve(zfn.call,0,1);
            std::cerr << "\tz[" << i << "]=" << zs << std::endl;
            const double p = Perimeter(zs);
            if(p>max_perimeter) max_perimeter = p;
            pSlice       pS(new Slice(zs,p) );
            Slices.push_back(pS);
        }

        std::cerr << "-- Max Perimeter=" << max_perimeter << std::endl;

        //______________________________________________________________________
        //
        // pass 2: compute how many points per slices M = 2+2*n
        //______________________________________________________________________
        std::cerr << "\t*** Finding Radial Slicing..." << std::endl;
        const double delta = 1.0/(N+1);
        const size_t n = max_of<size_t>(2,ceil(max_perimeter/delta))-2;
        const size_t M = 2+2*n;
        std::cerr << "\t\tn=" << n << ", M=" << M << std::endl;

        std::cerr << "\t*** Computing Points..." << std::endl;
        pPoint p0( new Point() );
        Points.push_back(p0);
        for(size_t i=1;i<=N;++i)
        {
            Slice &slice = *Slices[i];

            for(size_t j=0;j<M;++j)
            {
                const double theta = (j*numeric<double>::two_pi)/M;
                pPoint pp( new Point() );
                pp->r = gen_vtx(theta,slice.z);
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

        std::cerr << "\t\t#Triangles=" << Triangles.size() << std::endl;
        std::cerr << "\t\t#Points   =" << Points.size()    << std::endl;

        std::cerr << "\t*** Rescaling" << std::endl;
        for(size_t i=1; i <= Points.size();++i)
        {
            Point &p = *Points[i];
            p.r.z -= 0.5;
        }


    }

    inline ~Fish() throw()
    {
    }

    derivative<double> drvs;
    double             drvs_h;
    double             ztmp;
    integrator<double> intp;
    integrator<double> ints;


    inline double Surface(double z1,double z2)
    {
        return ints(z1,z2,Perimeter,FTOL);
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

    inline double __X( double theta )
    {
        return Width(ztmp) * cos(theta);
    }

    inline double __Y( double theta )
    {
        const double u = Up(ztmp);
        const double d = Down(ztmp);
        const double ratio = 0.5*(1+sin(theta));
        const double m     = Mix(ratio);
        return (u*m+d*(1-m))*sin(theta);
    }

    inline double __dPerimeter( double theta )
    {
        const double dX = drvs(X,theta,FTOL);
        const double dY = drvs(Y,theta,FTOL);
        return Hypotenuse(dX,dY);
    }

    inline double __dLength(double z)
    {
        return Hypotenuse(1.0, drvs(Width,z,FTOL) );
    }

    inline double __Perimeter( double z )
    {
        ztmp = z;
        return intp(0.0,numeric<double>::two_pi,dPerimeter,FTOL);
    }



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
    Lua::Function<double> Width(L,"Width",true);
    Lua::Function<double> Up(L,"Up",true);
    Lua::Function<double> Down(L,"Down",true);
    Lua::Function<double> Mix(L,"Mix",true);
    const size_t N = size_t(Lua::Config::Get<lua_Number>(L, "N"));


    Fish fish(Width,Up,Down,Mix,N);

    fish.save_vtk("fish.vtk");
    fish.save_stl("fish.stl");

}
YOCTO_PROGRAM_END()

