#include "yocto/sequence/vector.hpp"
#include "yocto/sequence/many-arrays.hpp"
#include "yocto/program.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/math/fcn/zfind.hpp"

using namespace yocto;
using namespace math;

static const size_t NVAR = 2;
static double       ATOL = 1e-5;
static double       BTOL = 1e-4;
static double       dY   = 1e-4;
static double       AMIN = 0.001;
typedef array<double> array_t;


class Bridge
{
public:

    double K;      //!< kappa * R
    double theta;  //!< wetting angle
    double beta;   //!< h/R
    many_arrays<double,memory::global> arrays;
    array_t &U;
    array_t &k1;
    array_t &k2;
    array_t &k3;
    array_t &k4;
    array_t &V;

    Bridge() :
    K(1),
    theta(0),
    beta(0),
    arrays(12),
    U(  arrays.next_array() ),
    k1( arrays.next_array() ),
    k2( arrays.next_array() ),
    k3( arrays.next_array() ),
    k4( arrays.next_array() ),
    V(  arrays.next_array() )
    {
        arrays.allocate(NVAR);
    }

    ~Bridge() throw()
    {
    }

    inline void Equation(array<double>       &dudy,
                         const double         y,
                         const array<double> &u) throw()
    {
        const double r     = u[1];
        const double drdy  = u[2];
        const double speed = sqrt(1.0+drdy*drdy);
        const double accel = ((K*K*y)+1.0/r/speed)*speed*speed*speed;
        dudy[1] = drdy;
        dudy[2] = accel;
    }

    bool IsValid(const double r,const double y) const throw()
    {
        if(isinf(r))
        {
            //std::cerr << "INFINITE" << std::endl;
            return false;
        }

        if(isnan(r))
        {
            //std::cerr << "NaN" << std::endl;
            return false;
        }

        if(r<=0)
        {
            //std::cerr << "NEGATIVE R" << std::endl;
            return false;
        }

        {
            const double bridge_r = 0;
            const double bridge_y = beta + 1.0;
            const double dr       = r - bridge_r;
            const double dy       = y - bridge_y;
            const double l2       = dr*dr + dy*dy;
            if(l2<1.0)
            {
                //std::cerr << "INSIDE BRIDGE" << std::endl;
                return false;
            }
        }

        return true;
    }


    inline bool RK4(const double y_ini,
                    const double y_end) throw()
    {
        const double h     = y_end - y_ini;
        const double half  = h*0.5;
        const double y_mid = y_ini + half;
        Equation(k1,y_ini,U);

        tao::setprobe(V, U, half, k1);
        Equation(k2,y_mid,V);

        tao::setprobe(V,U,half,k2);
        Equation(k3,y_mid,V);

        tao::setprobe(V,U,h,k3);
        Equation(k4,y_end,V);

        for(size_t i=U.size();i>0;--i)
        {
            U[i] += h * (k1[i]+k2[i]+k2[i]+k3[i]+k3[i]+k4[i]) / 6.0;
        }

        return IsValid(U[1],y_end);
    }

    inline void OutputBridge() const
    {
        const size_t  NB = 100;
        ios::ocstream fp("bridge.dat",false);
        for(size_t i=0;i<=NB;++i)
        {
            const double angle = (numeric<double>::two_pi*i)/NB;
            fp("%g %g\n",sin(angle),beta+(1.0-cos(angle)));
        }
    }

    inline void OutputTangents() const
    {
        ios::ocstream fp("tangents.dat",false);
        for(int alpha=0;alpha<=180;alpha+=15)
        {
            const double aa    = numeric<double>::pi * double(alpha)/180.0;
            const double angle = numeric<double>::pi * ( double(alpha)+theta) / 180.0;
            const double tr    = -cos(angle);
            const double tz    = -sin(angle);
            const double r_c   = sin(aa);
            const double z_c   = beta + (1-cos(aa));
            const double len   = 0.2;
            fp("%g %g\n%g %g\n\n",r_c,z_c,r_c+len*tr,z_c+len*tz);
        }

    }

    inline double GetLastRadius() const throw() { return U[1]; }

    inline bool ComputeFinalRadius(const double alpha)
    {

        //std::cerr << "FinalRadius(" << alpha << ").."; std::cerr.flush();
        bool success = true;

        if(theta+alpha>=180.0)
        {
            success = false;
        }
        else
        {
            const double alpha_rad = numeric<double>::pi * alpha / 180.0;
            const double theta_rad = numeric<double>::pi * theta / 180.0;

            // contact coordinates
            const double u_c  = sin(alpha_rad);
            const double y_c  = beta + (1.0-cos(alpha_rad));
            const double dudy = 1.0/tan(alpha_rad+theta_rad);


            // number of step
            const size_t ny   = max_of<size_t>( size_t( Ceil(y_c/dY) + 1), 10 );

            // initial condition
            U[1] = u_c;
            U[2] = dudy;
            double reg[3] = { U[1],0,0 };
            size_t num    = 1;
            for(size_t i=ny;i>0;--i)
            {
                const double y_ini = (i  )*y_c/ny;
                const double y_end = (i-1)*y_c/ny;
                if(!RK4(y_ini,y_end))
                {
                    success = false;
                    break;
                }
                if(num<3)
                {
                    reg[num++] = U[1];
                }
                else
                {
                    reg[0] = reg[1];
                    reg[1] = reg[2];
                    reg[2] = U[1];
                    if((reg[1]+reg[1])>= (reg[0]+reg[2]) )
                    {
                        success = false;
                        break;
                    }
                }
            }
        }

        //if(success) {std::cerr << "..OK" << std::endl;} else {std::cerr << "..NO" << std::endl;}

        return success;
    }


    inline void SaveProfile(const string &filename,const double alpha)
    {
        ios::ocstream fp(filename,false);
        if(theta+alpha>=180.0) return;

        const double alpha_rad = numeric<double>::pi * alpha / 180.0;
        const double theta_rad = numeric<double>::pi * theta / 180.0;

        // contact coordinates
        const double u_c  = sin(alpha_rad);
        const double y_c  = beta + (1.0-cos(alpha_rad));
        const double dudy = 1.0/tan(alpha_rad+theta_rad);


        // number of step
        const size_t ny   = max_of<size_t>( size_t( Ceil(y_c/dY) + 1), 10 );

        // initial condition
        U[1] = u_c;
        U[2] = dudy;
        //fp("%g %g\n", u_c, y_c);
        for(size_t i=ny;i>0;--i)
        {
            const double y_ini = (i  )*y_c/ny;
            const double y_end = (i-1)*y_c/ny;
            if(!RK4(y_ini,y_end))
            {
                break;
            }
            if(U[1]<4)
            {
                fp("%g %g\n",U[1],y_end);
            }
        }

    }

    void ScanAlpha()
    {
        //std::cerr << "ScanAlpha(K=" << K << ",theta=" << theta << ",beta=" << beta <<")" << std::endl;

        ios::ocstream fp( "alpha.dat", false);
        const int alpha_max = int(ceil(180-theta));
        for(int alpha=0;alpha<=alpha_max;++alpha)
        {
            double ans = 0;
            if( ComputeFinalRadius(alpha) )
            {
                ans = 1.0/GetLastRadius();
            }
            fp("%d %g\n", alpha, ans);
        }

    }

    inline double ComputeAlpha()
    {
        //std::cerr << "FindAlpha(K=" << K << ",theta=" << theta << ",beta=" << beta <<")" << std::endl;
        double hi = 180 - theta; // assuming invalid
        double lo = hi/2;        // we don't know

        //----------------------------------------------------------------------
        // bracket
        //----------------------------------------------------------------------
        while(true)
        {
            if(ComputeFinalRadius(lo))
            {
                break;
            }
            else
            {
                hi  = lo;
                lo /=  2;
                //std::cerr << "lo=" << lo << std::endl;
                if(lo<=AMIN)
                {
                    return -1; // not found
                }

            }
        }
        //std::cerr << "\tbracket_alpha: " << lo << " -> " << hi << std::endl;

        //----------------------------------------------------------------------
        // bissect
        //----------------------------------------------------------------------
        while(hi-lo>ATOL)
        {
            const double mid = 0.5*(lo+hi);
            if(ComputeFinalRadius(mid))
            {
                lo = mid;
            }
            else
            {
                hi = mid;
            }
        }
        (void)ComputeFinalRadius(lo);
        return lo;
    }

    inline double ComputeSurface()
    {
        const double alpha = ComputeAlpha();
        if(alpha<0)
            return -1;
        else
        {
            const double aa = numeric<double>::pi * alpha / 180.0;
            const double sa = Sin(aa);
            return numeric<double>::pi * sa*sa;
        }
    }

    inline double ComputeBetaMax()
    {
        double blo = 0;     //! assume good
        beta = blo;
        if(ComputeAlpha()<0)
        {
            return -1;
        }

        const double dBeta = 1.0/K;
        double bhi = dBeta;
        beta = bhi;
        double alpha = 0;
        while( (alpha=ComputeAlpha()) >=0 )
        {
            //std::cerr << "alpha(" << beta << ")=" << alpha << std::endl;
            beta += dBeta;
            //std::cerr << "beta=" << beta << std::endl;
        }
        bhi  = beta;

        // blo=good, bhi=bad
        while(bhi-blo>BTOL)
        {
            //std::cerr << "blo=" << blo << ", bhi=" << bhi << std::endl;
            const double mid = (blo+bhi)*0.5;
            beta = mid;
            if( ComputeAlpha() < 0 )
            {
                bhi = mid;
            }
            else
            {
                blo = mid;
            }
        }

        beta = blo;
        return beta;
    }

    double AlphaOf(double Theta)
    {
        theta = Theta;
        return ComputeAlpha();
    }


private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Bridge);
};


#include "yocto/math/io/data-set.hpp"
#include "yocto/ios/icstream.hpp"

class DataFile
{
public:
    explicit DataFile(const string &filename) :
    Height(),
    Surface(),
    Length(0),
    beta(),
    alpha()
    {
        data_set<double> ds;
        ds.use(1, Height);
        ds.use(2, Surface);
        ios::icstream fp(filename);
        ds.load(fp);
        (size_t &)Length = Height.size();
        beta.make(Length,0);
        alpha.make(Length,0);
        std::cerr << "Length=" << Length << std::endl;
    }

    void Normalize(const double R)
    {
        const double area = numeric<double>::pi*R*R;
        for(size_t j=Length;j>0;--j)
        {
            beta[j] = Height[j]/R;
            const double sa = clamp<double>(0,Surface[j]/area,1.0);
            alpha[j] = 180.0 * (Asin(sa)/numeric<double>::pi);
        }
    }

    void Inverse(Bridge &B)
    {

        zfunction<double> zfn(&B, &Bridge::AlphaOf, 0);
        for(size_t i=Length;i<=Length;++i)
        {
            B.beta = beta[i];
            zfn.target = 0;
            std::cerr << "alpha[" << i << "]=" << alpha[i] << std::endl;
            const double theta_max = 180.0 - alpha[i];
            ios::ocstream fp("alpha-theta.dat",false);
            for(double theta=0;theta<theta_max;theta+=0.1)
            {
                fp("%g %g\n", theta, zfn.call(theta) );
            }
            break;
        }
    }

    virtual ~DataFile() throw()
    {
    }

    vector<double> Height;
    vector<double> Surface;
    const size_t   Length;
    vector<double> beta;
    vector<double> alpha;

private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(DataFile);
};


#include "yocto/string/conv.hpp"
#include "yocto/fs/local-fs.hpp"

YOCTO_PROGRAM_START()
{
#if 0
    if(argc<=3)
    {
        throw exception("need K theta beta");
    }
    Bridge b;
    b.K     = strconv::to<double>(argv[1],"K");
    b.theta = strconv::to<double>(argv[2],"theta");
    b.beta  = strconv::to<double>(argv[3],"beta");

    //b.OutputBridge();
    //b.OutputTangents();
    //b.ScanAlpha();
    //const double alpha = b.ComputeAlpha();
    //std::cerr << "alpha=" << alpha << std::endl;
    std::cerr << "beta_max=" << b.ComputeBetaMax() << std::endl;
#endif

#if 0
    if(argc<=1)
    {
        throw exception("need K");
    }
    Bridge b;
    b.K = strconv::to<double>(argv[1],"K");
    const string fn = vformat("beta_max-K%g.dat",b.K);
    ios::ocstream::overwrite(fn);
    for(int theta = 5; theta <= 175; theta += 5)
    {
        std::cerr << "theta=" << theta << std::endl;
        b.theta = theta;
        const double bmax = b.ComputeBetaMax();
        b.beta = bmax;

        ios::ocstream fp(fn,true);
        fp("%g %g\n",b.theta,bmax);
    }
#endif

#if 1
    if(argc<=2)
    {
        throw exception("need R[mm], 1/kappa [mm]");
    }
    const double R     = strconv::to<double>(argv[1],"R");
    const double kappa = 1.0/strconv::to<double>(argv[2],"1/kappa");
    Bridge       B;
    B.K = R*kappa;


    for(int i=3;i<argc;++i)
    {
        const string filename = argv[i];
        DataFile     datafile(filename);
        datafile.Normalize(R);
        string bname = vfs::get_base_name(filename);
        bname += ".dat";
        {
            ios::ocstream fp(bname,false);
            for(size_t j=1;j<=datafile.Length;++j)
            {
                fp("%g %g %g %g\n",datafile.Height[j], datafile.Surface[j], datafile.beta[j], datafile.alpha[j]);
            }
        }
        datafile.Inverse(B);
    }
#endif
    
}
YOCTO_PROGRAM_END()
