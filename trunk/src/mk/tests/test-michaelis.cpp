#include "yocto/utest/run.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

#include <typeinfo>
#include <cmath>


using namespace yocto;
using namespace math;


#include "yocto/code/rand.hpp"

namespace
{
    class Michaelis
    {
    public:
        double k1,k2,k3,k4;
        double beta;
        
        inline Michaelis() :
        k1(0),
        k2(0),
        k3(0),
        k4(0),
        beta(0)
        {
            k1 = 1   * (1+alea<double>());
            k2 = 0.1 * (1+alea<double>());
            k3 = 0.2 * (1+alea<double>());
            k4 = 0.08;
            
            beta = (k4*k2)/(k1*k3);
        }
        
        void show_steady( const array<double> &y ) const
        {
            const double & S  = y[1];
            const double & E  = y[2];
            const double & _ES = y[3];
            const double & P  = y[4];
            const double E0 = E + _ES;
            const double S0 = S + _ES + P;
            
            const double a  = k1*(1+beta);
            const double b  = k2+beta*(k2+k2*(E0-S0));
            const double c  = -beta*k2*S0;
            
            const double delta = b*b - 4.0 * (a*c);
            const double Seq   = (sqrt(delta) - b)/(a+a);
            std::cerr << "[S]eq=" << Seq << std::endl;
            
            const double ESeq  = E0 * (k1*Seq)/(k2+k1*Seq);
            std::cerr << "[ES]eq=" << ESeq << std::endl;
            
        }
        
        void rate( array<double> &dydx, double , const array<double> &y )
        {
            const double & S  = y[1];
            const double & E  = y[2];
            const double & _ES = y[3];
            const double & P  = y[4];
            
            const double v1 = k1 * S * E;
            const double v2 = k2 * _ES;
            const double v3 = k3 * _ES;
            const double v4 = k4 * P * E;
            
            double & dSdt  = dydx[1];
            double & dEdt  = dydx[2];
            double & dESdt = dydx[3];
            double & dPdt  = dydx[4];
            
        
            dSdt  = (v2-v1);
            dEdt  = (v2+v3) - (v1+v4);
            dESdt = (v1+v4) - (v2+v3);
            dPdt  = v3-v4;
            
        }
        
        inline ~Michaelis()
        {
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Michaelis);
    };
    
}

static const char FileName[] = "mm.dat";

static inline void  Output( double t, const array<double> &C)
{
    ios::ocstream fp(FileName,true);
    fp("%g",t);
    for(size_t i=1; i <= C.size(); ++i)
    {
        fp(" %g", C[i]);
    }
    fp("\n");
}


YOCTO_UNIT_TEST_IMPL(michaelis)
{
   
    ode::driverCK<double>::type       odeint(1e-4);
    Michaelis                         enzyme;
    ode::Field<double>::Equation      diffeq( &enzyme, & Michaelis::rate );

    const size_t   nvar = 4;
    vector<double> y(nvar,0);
    odeint.start(nvar);
    
    double &S  = y[1];
    double &E  = y[2];
    double &_ES = y[3];
    double &P  = y[4];
    
    S  = 1;
    E  = 0.1 + 0.5*(1+alea<double>());
    _ES = 0;
    P  = 0;
    
    std::cerr << "Steady State:" << std::endl;
    enzyme.show_steady(y);
    
    double       t  = 0;
    const double dt = 1e-2;
    double       h  = t/10;
    ios::ocstream::overwrite(FileName);
    Output(t, y);
    for(size_t i=1;;++i)
    {
        odeint( diffeq, y, t, t+dt, h, NULL);
        t=i*dt;
        Output(t, y);
        if(t>30) break;
    }
    
}
YOCTO_UNIT_TEST_DONE()

