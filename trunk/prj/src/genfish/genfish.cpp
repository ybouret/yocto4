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

class Fish
{
public:
    Function       W;
    Function       H;
    const size_t   N;    //!< internal slices
    vector<pPoint> Points;
    vector<pSlice> Slices;
    const double   ftol;
    const double   delta;
    
    explicit Fish(const Function &userW,
                  const Function &userH,
                  const size_t    userN) :
    W(userW),
    H(userH),
    N(max_of<size_t>(userN,1)),
    Points(),
    Slices(),
    ftol(1e-4),
    delta(1.0/(N+1)),
    drvs(),
    drvs_h( 1e-4 ),
    intg(),
    dPerimeter(this,& Fish:: __dPerimeter),
    __a(0),
    __b(0),
    __s(0),
    Perimeter(this, & Fish:: __perimeter ),
    Surface(this, & Fish:: __surface),
    TotalSurface( Surface(1.0) ),
    DeltaSurface(TotalSurface*delta)
    {
        
        
        //-- make slice 0
        
        //-- find locations
        zfind<double> solve(1e-4);
        Function zS(this,&Fish::zSurface);
        double max_perimeter = 0;
        for(size_t i=1;i<=N;++i)
        {
            __s = i*DeltaSurface;
            const double z = solve(zS,0,1);
            std::cerr << "slice #" << i << "@" << z << std::endl;
            const double pr = Perimeter(z);
            std::cerr << "\tperimeter=" << pr << std::endl;
            pSlice ps( new Slice(z,pr) );
            Slices.push_back(ps);
            if(pr>max_perimeter) max_perimeter = pr;
#if 0
            for(size_t j=0;j<M;++j)
            {
                const double theta = (j * 6.28) / double(M);
                __a   = W(z);
                __b   = H(z);
                const double ct = cos(theta);
                const double st = sin(theta);
                const double x =   __a * ct + __b * st;
                const double y = - __a * st + __b * ct;
                pPoint pp( new Point() );
                pp->r.x = x;
                pp->r.y = y;
                pp->r.z = z;
                Points.push_back(pp);
                ps->points.push_back(pp);
            }
#endif
        }
        
        //-- make slice N+1
        
        
        std::cerr << "#Slices=" << Slices.size() << std::endl;
        
        //-- compute 2+2*n points per internal slices
        const size_t n = (max_of<size_t>(2,ceil(max_perimeter/delta))-2)/2;
        std::cerr << "n=" << n << std::endl;
        const size_t M = 2 + 2*n;
        std::cerr << "M=" << M << " points per slices" << std::endl;
        
        const size_t NS = Slices.size();
        for(size_t j=1;j<=NS;++j)
        {
            Slice &slice = *Slices[j];
            const double z = slice.z;
            __a   = W(z);
            __b   = H(z);
            
            for(size_t i=0;i<M;++i)
            {
                const double theta = (i * numeric<double>::two_pi) / double(M);
                const double ct = cos(theta);
                const double st = sin(theta);
                const double x =   __a * ct;
                const double y =   __b * st;
                pPoint pp( new Point() );
                pp->r.x = x;
                pp->r.y = y;
                pp->r.z = z;
                Points.push_back(pp);
                slice.points.push_back(pp);
            }
        }
        std::cerr << "#Points=" << Points.size() << std::endl;
        
    }
    
    virtual ~Fish() throw()
    {
        
    }
    
    
    void save_vtk( const string &filename ) const
    {
        const size_t NP = Points.size();
        const size_t NS = Slices.size();
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
        
    }
    
    
private:
    derivative<double> drvs;
    const double       drvs_h;
    integrator<double> intg;
    Function           dPerimeter;
    
    double __a;
    double __b;
    double __s;
    
    double __dPerimeter( double theta )
    {
        return Hypotenuse( __a * cos(theta), __b * sin(theta) );
    }
    
    double __perimeter(double z)
    {
        __a = W(z);
        __b = H(z);
        
        return intg(0,numeric<double>::two_pi,dPerimeter,ftol);
    }
    
    double __surface(double z)
    {
        if(z<=0)
            return 0;
        else
        {
            return intg(0,min_of<double>(z,1.0), Perimeter, ftol);
        }
    }
    
    double zSurface( double z )
    {
        return __surface(z) - __s;
    }
    
    
    YOCTO_DISABLE_COPY_AND_ASSIGN(Fish);
    
public:
    Function     Perimeter;
    Function     Surface; // cumulative surface
    const double TotalSurface;
    const double DeltaSurface;
};

static inline double H0(double z)
{
    return 0.3*sqrt(z*(1-z)*(1-z)*(1-z));
}

static inline double W0(double z)
{
    return 0.4*sqrt(z*(1-z)*(1-z));
}


int main(int argc, char *argv[] )
{
    static const char *program = vfs::get_base_name(argv[0]);
    try
    {
        Function WW = cfunctor(W0);
        Function HH = cfunctor(H0);
        
        Fish F(WW,HH,50);
        
        std::cerr << "TotalSurface=" << F.TotalSurface << std::endl;
        
        F.save_vtk("fish.vtk");
        return 0;
    }
    catch(...)
    {
        std::cerr << "Uncaught exception in " << program << std::endl;
    }
    return 1;
}
