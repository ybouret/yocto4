#include "yocto/sequence/vector.hpp"
#include "yocto/program.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace math;

static const size_t NVAR = 2;
static double       ATOL = 1e-4;
static double       BTOL = 1e-4;

class Bridge
{
public:
    double         K;     //!< kappa*R
    double         beta;  //!< h/R
    double         theta; //!< angle in degrees !
    double         dY;    //!< integration step
    vector<double> U;
    vector<double> k1;
    vector<double> k2;
    vector<double> k3;
    vector<double> k4;
    vector<double> V;
    
    explicit Bridge(const double user_K,
                    const double user_theta,
                    const double user_dY) :
    K(user_K),
    beta(0),
    theta(user_theta),
    dY(max_of<double>(1e-9,Fabs(user_dY))),
    U(NVAR,0.0),
    k1(NVAR,0.0),
    k2(NVAR,0.0),
    k3(NVAR,0.0),
    k4(NVAR,0.0),
    V(NVAR,0.0)
    {
        //std::cerr << "K=" << K << std::endl;
    }
    
    void OutputBridge() const
    {
        const size_t  NB = 100;
        ios::ocstream fp("bridge.dat",false);
        for(size_t i=0;i<=NB;++i)
        {
            const double angle = (numeric<double>::two_pi*i)/NB;
            fp("%g %g\n",sin(angle),beta+(1.0-cos(angle)));
        }
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
            if(l2<1)
            {
                //std::cerr << "INSIDE BRIDGE" << std::endl;
                return false;
            }
        }
        
        return true;
    }
    
    void DiffEq(array<double>       &dudy,
                const double         y,
                const array<double> &u)
    {
        const double r    = u[1];
        const double drdy = u[2];
        const double speed = sqrt(1.0 + drdy*drdy);
        const double accel = ((K*K*y)+1.0/r/speed)*speed*speed*speed;
        dudy[1] = drdy;
        dudy[2] = accel;
    }
    
    bool RK4(const double y_ini,
             const double y_end)
    {
        const double h     = y_end - y_ini;
        const double half  = h*0.5;
        const double y_mid = y_ini + half;
        DiffEq(k1,y_ini,U);
        
        tao::setprobe(V, U, half, k1);
        DiffEq(k2,y_mid,V);
        
        tao::setprobe(V,U,half,k2);
        DiffEq(k3,y_mid,V);
        
        tao::setprobe(V,U,h,k3);
        DiffEq(k4,y_end,V);
        
        for(size_t i=U.size();i>0;--i)
        {
            U[i] += h * (k1[i]+k2[i]+k2[i]+k3[i]+k3[i]+k4[i]) / 6.0;
        }
        
        return IsValid(U[1],y_end);
    }
    
    bool ComputeFinalRadius(const double alpha)
    {
        
        //ios::ocstream fp("profile.dat",false);
        
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
        bool success = true;
        for(size_t i=ny;i>0;--i)
        {
            const double y_ini = (i  )*y_c/ny;
            const double y_end = (i-1)*y_c/ny;
            if(!RK4(y_ini,y_end))
            {
                success = false;
                break;
            }
            if(U[1]<4)
            {
                //fp("%g %g\n",U[1],y_end);
            }
        }
        
        
        return success;
    }
    
    
    void ScanAlpha()
    {
        ios::ocstream fp("alpha.dat",false);
        for(int alpha=0;alpha<=180;++alpha)
        {
            double val = 0;
            if(ComputeFinalRadius(alpha))
            {
                val = 1.0/U[1];
            }
            fp("%g %g\n",double(alpha),val);
        }
    }
    
    double FindLowerAlpha()
    {
        for(int alpha=179;alpha>=1;--alpha)
        {
            if(ComputeFinalRadius(alpha))
            {
                return alpha;
            }
        }
        //std::cerr << "No Lower" << std::endl;
        return -1;
    }
    
    double FindAlpha()
    {
        double lower = FindLowerAlpha();
        if(lower<0)
        {
            return -1;
        }
        
        double upper = lower + 1;
        while( (upper-lower)>ATOL )
        {
            const double middle = 0.5*(upper+lower);
            if(ComputeFinalRadius(middle))
            {
                lower = middle;
            }
            else
            {
                upper = middle;
            }
        }
        
        const double alpha = lower;
        if(!ComputeFinalRadius(alpha))
        {
            
            return -1;// unexpected
        }
        
        return alpha;
    }
    
    inline double FindBetaMax()
    {
        std::cerr << "Finding Beta Max K=" <<  K << ", theta=" << theta << std::endl;
        beta = 0;
        double alpha = FindAlpha();
        if(alpha<0)
        {
            return -1;
        }
        std::cerr << "alpha0=" << alpha << std::endl;
        
        
        // find a beta with invalid alpha
        beta = 1.0/K;
        while( FindAlpha()>=0 ) beta *= 2;
        std::cerr << "start@beta=" << beta << std::endl;
        
        double lower = 0;
        double upper = beta;
        while( (upper-lower)>BTOL )
        {
            const double middle = 0.5*(lower+upper);
            beta  = middle;
            alpha = FindAlpha();
            if(alpha<0)
            {
                upper = middle;
            }
            else
            {
                lower = middle;
                std::cerr << "beta..." << lower << std::endl;
            }
        }
        beta   = lower;
        alpha  = FindAlpha();
        
        return beta;
    }
    
    inline ~Bridge() throw()
    {
        
    }
    
    double Alpha0( const double user_K )
    {
        beta = 0;
        K = user_K;
        return FindAlpha();
    }
    
  
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Bridge);
};

#include "yocto/string/conv.hpp"

YOCTO_PROGRAM_START()
{
    
#if 1
    {
        Bridge b(1,0,1e-4);
        const double beta_max = b.FindBetaMax();
        std::cerr << "beta_max_null(K=1)=" << beta_max << std::endl;
    }
    
    const double lnKmin   = -4.7;
    const double lnKmax   =  4.7;
    const int    nk       =  50;
    const string filename = "beta_max_null.dat";
    
    ios::ocstream::overwrite(filename);
    for(int k=nk;k>=0;--k)
    {
        const double lnK = lnKmin + ( k*(lnKmax-lnKmin) )/nk;
        const double K   = exp(lnK);
        Bridge b(K,0,1e-4);
        const double beta_max = b.FindBetaMax();
        ios::ocstream fp(filename,true);
        fp("%g %g %g\n",lnK,beta_max,K);
    }
#endif
    
#if 0
    if(argc<=1)
    {
        throw exception("need K");
    }
    const double K        = strconv::to<double>(argv[1],"K");
    const string filename = vformat("beta_max%g.dat",K);
    
    ios::ocstream::overwrite(filename);
    for(int theta=0;theta<=175;theta+=5)
    {
        Bridge b(K,theta,1e-4);
        const double beta_max = b.FindBetaMax();
        ios::ocstream fp(filename,true);
        fp("%g %g\n",b.theta,beta_max);
    }
#endif
    
#if 0
    if(argc<=2)
    {
        throw exception("need K theta");
    }
    const double K     = strconv::to<double>(argv[1],"K");
    const double theta = strconv::to<double>(argv[2],"theta");
    Bridge b(K,theta,1e-4);
    const double beta_max = b.FindBetaMax();
    
    std::cerr << std::endl << "beta_max=" << beta_max << std::endl;
    const size_t nbeta    = 40;
    const string filename = vformat("alpha-K=%g-theta=%g.dat",K,theta);
    ios::ocstream::overwrite(filename);
    for(size_t i=0;i<=nbeta;++i)
    {
        b.beta = (i*beta_max)/nbeta;
        const double alpha = b.FindAlpha();
        ios::ocstream fp(filename,true);
        fp("%g %g\n",b.beta,alpha);
    }
#endif
    
    
    
}
YOCTO_PROGRAM_END()

