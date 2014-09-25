#include "yocto/utest/run.hpp"
#include "yocto/fame/box.hpp"
#include "yocto/fame/integrate.hpp"

#include "yocto/physics/constants.hpp"
#include "yocto/math/types.hpp"

#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace fame;
using namespace math;

namespace {
    
    
    class Cell : public layout1D
    {
    public:
        box<1,double>   B;
        array1D<double> X;
        array1D<double> Psi;
        array1D<double> dPsi;
        array1D<double> Na;
        array1D<double> Cl;
        array1D<double> Cs;
        const double    dX;
        double          I;
        double          lambda;
        double          lam0;
        const double    epsilon;
        const double    alpha;
        const double    dX2;
        double          CNa;
        double          CCl;
        
        explicit Cell( const layout1D l, double length) :
        layout1D(l),
        B(0,length),
        X( "X", *this ),
        Psi("Psi",*this),
        dPsi("dPsi", *this),
        Na( "Na", *this),
        Cl( "Cl", *this),
        Cs( "Cs", *this),
        dX(B.length/(items-1)),
        I(0),
        lambda(0),
        lam0(0),
        epsilon( Y_EPSILON0 * 78.5 ),
        alpha( (1000.0 * Y_FARADAY) / epsilon),
        dX2(dX*dX),
        CNa(0),
        CCl(0)
        {
            B.map(X);
            std::cerr << "B="     << B     << std::endl;
            std::cerr << "X="     << X     << std::endl;
            std::cerr << "dX="    << dX    << std::endl;
            std::cerr << "alpha=" << alpha << std::endl;
            
            lam0 = Sqrt( (Y_R*298*epsilon)/(Square(Y_FARADAY)*1000.0) );
            std::cerr << "lam0=" << lam0  << std::endl;
        }
        
        virtual ~Cell() throw()
        {
        }
        
        void compute_lambda()
        {
            CNa = int1D(X, Na)/B.space;
            CCl = int1D(X, Cl)/B.space;
            I = (CNa+CCl)/2;
            const double ksq = 2000.0 * Square(Y_FARADAY)/(Y_R*298*epsilon) * I;
            lambda = 1.0/Sqrt(2*ksq);
            std::cerr << "lambda=" << lambda << std::endl;
        }
        
        
        double Laplacian(unit_t i) const throw()
        {
            const double mid = Psi[i];
            return (Psi[i-1] - (mid+mid) + Psi[i+1]) / dX2;
        }
        
        double getErr0(unit_t i) const throw()
        {
            return Psi[i] - (lambda*lambda) * Laplacian(i);
        }
        
        double getRMS0() const throw()
        {
            double rms = 0;
            for(unit_t i=lower+1;i<upper;++i)
            {
                rms += Square(getErr0(i));
            }
            return Sqrt(rms/items);
        }
        
        double rb0()
        {
            const double factor = 1.0/(2.0+Square(dX/lambda));
            for(unit_t i=lower+1;i<upper;i += 2)
            {
                Psi[i] = factor*(Psi[i-1]+Psi[i+1]);
            }
            
            for(unit_t i=lower+2;i<upper;i += 2)
            {
                Psi[i] = factor*(Psi[i-1]+Psi[i+1]);
            }
            return getRMS0();
        }
        
        static inline double weight( double v, int z )  throw()
        {
            return exp(-(z*Y_FARADAY*v)/(Y_R*298) );
        }
        
        void dispatch( array1D<double> &C, const int z)
        {
            const double Ntot = int1D(X,C);
            double nrm = 0;
            for(unit_t i=lower;i<=upper;++i)
            {
                const double w = weight(Psi[i],z);
                C[i] = Ntot * w;
                nrm += w;
            }
            nrm *= dX;
            
            for(unit_t i=lower;i<=upper;++i)
            {
                C[i] /= nrm;
            }
            
            const double Nfin = int1D(X,C);
            const double coef = Ntot/Nfin;
            for(unit_t i=lower;i<=upper;++i)
            {
                C[i] *= coef;
            }
        }
        
        void solve0()
        {
            
            double old_rms = rb0();
            double new_rms = old_rms;
            
            while( (new_rms=rb0()) >= old_rms )
                ;
            
            while( (new_rms=rb0()) < old_rms )
            {
                old_rms = new_rms;
                std::cerr << "rms=" << new_rms << std::endl;
            }
            dispatch( Na,1);
            dispatch( Cl,-1);
        }
        
        
        void compute_Cs()
        {
            for(unit_t i=lower;i<=upper;++i)
            {
                Cs[i] = -alpha*(Na[i] - Cl[i]);
            }
        }
        
        double getErr1(unit_t i) const throw()
        {
            return Laplacian(i) - Cs[i];
        }
        
        double getRMS1() const throw()
        {
            double rms = 0;
            for(unit_t i=lower+1;i<upper;++i)
            {
                rms += Square(getErr1(i));
            }
            return Sqrt(rms/items);
        }
        
        
        void solveNewton(unit_t i)
        {
            const double fac = Y_FARADAY/(Y_R*298);
            const double y   =  fac * (Psi[i-1]+Psi[i+1]);
            
            double zeta_i    =  fac * Psi[i];
            const  double ratio = Square(dX/lam0);
            
            for(;;)
            {
                double f  = zeta_i+zeta_i - ratio * (CNa*exp(-zeta_i) - CCl*exp(zeta_i));
                double fp = 2 + ratio * (CNa*exp(-zeta_i)+CCl*exp(zeta_i));
                const double zold = zeta_i;
                zeta_i += (y-f)/fp;
                const double dzeta = zold-zeta_i;
                if(Fabs(dzeta)<=numeric<double>::ftol*(Fabs(zeta_i)) )
                {
                    break;
                }
            }
            std::cerr << "zeta[" << i << "]=" << zeta_i << std::endl;
            Psi[i] = zeta_i/fac;
        }
        
        double rb1()
        {
            for(unit_t i=lower+1;i<upper;i += 2)
            {
                solveNewton(i);
            }
            
            for(unit_t i=lower+2;i<upper;i += 2)
            {
                solveNewton(i);
            }
            
            return 0;
        }
        
        void solve1()
        {
            CNa = int1D(X, Na)/B.space;
            CCl = int1D(X, Cl)/B.space;
            std::cerr << "lam0=" << lam0 << std::endl;
            
            rb1();
            
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
    };
    
}


YOCTO_UNIT_TEST_IMPL(poisson)
{
    
    Cell cell( layout1D(0,5000), 1e-6 );
    
    cell.Na.ld(0.01);
    cell.Cl.ld(0.01);
    
    cell.compute_lambda();
    
    cell.Psi[cell.lower] = 60e-3;
    cell.Psi[cell.upper] = 0;
    
    cell.solve0();
    {
        ios::ocstream fp("Psi0.dat",false);
        for(unit_t i=cell.lower;i<=cell.upper;++i)
        {
            fp("%g %g %g %g\n", cell.X[i], cell.Psi[i], cell.Na[i], cell.Cl[i]);
        }
    }
    
    
    cell.solve1();
}
YOCTO_UNIT_TEST_DONE()
