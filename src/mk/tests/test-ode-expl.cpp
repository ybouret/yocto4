#include "yocto/utest/run.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"
#include "yocto/math/ode/explicit/driver-dp.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"
#include <typeinfo>

using namespace yocto;
using namespace math;

namespace
{
    template <typename T>
    class problem
    {
    public:
        explicit problem() throw() :
        a(1),
        count(0)
        {
        }
        
        virtual ~problem() throw()
        {
        }
        
        T      a;
        size_t count;
        
        void dExp( array<T> &dydx, T , const array<T> &y )
        {
            assert(dydx.size()==1);
            assert(y.size()==1);
            dydx[1] = -a * y[1];
            ++count;
        }
        
        void ExpCB( array<T> &, T )
        {
            //std::cerr << "exp_cb@" << x << " / " << y << std::endl;
        }
        
        void dCos( array<T> &dydx, T , const array<T> &y )
        {
            assert(dydx.size()==2);
            assert(y.size()==2);
            dydx[1] = y[2];
            dydx[2] = - a * y[1];
            ++count;
        }
        
        void dLV( array<T> &dydx, T , const array<T> &y )
		{
			const T a = 3, b = 2, c = 3, d = 2;
			const T X = y[1];
			const T Y = y[2];
			dydx[1] =  a*X - b * X * Y;
			dydx[2] = -c*Y + d * X * Y;
			++count;
		}

        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(problem);
    };
    
    
    template <typename T,
    typename SOLVER
    >
    static inline void solve( const char *rk )
    {
        const T eps = T(1e-7);
        SOLVER  odeint(eps);
        
        problem<T>                      pb;
        typedef typename ode::Field<T>::Equation equation;
        typedef typename ode::Field<T>::Callback callback;
        const char *id = typeid(T).name();
        
        std::cerr << "--------------------" << std::endl;
        std::cerr << "solving <" << id << ">/" << rk << std::endl;
        std::cerr << "--------------------" << std::endl;
        
        vector<T> y;
        
        
        const size_t n  = 100;
        T            L  = 10;
        T            h1 = L/n;
        
        std::cerr << "Exp" << std::endl;
        {
            equation EqExp( &pb, & problem<T>::dExp  );
            callback OnExp( &pb, & problem<T>::ExpCB );
            
            y.make(1,0);
            y[1]     = 1;
            pb.count = 0;
            odeint.start(1);
            {
                const string  fn =  vformat("%s-exp-%s.dat",  rk, id );
                ios::ocstream fp(fn,false);
                fp("%g %g\n", double(0), double(y[1]));
                
                for(size_t i=0;i<n;++i)
                {
                    const T x0 = (i*L)/n;
                    const T x1 = ((i+1)*L)/n;
                    odeint(EqExp,y,x0,x1,h1,&OnExp);
                    fp("%g %g\n", double(x1), double(y[1]));
                }
                
                std::cerr << "count=" << pb.count << std::endl;
            }
            
            
            
        }
        
        
        std::cerr << "Cos" << std::endl;
        {
            equation EqCos( &pb, & problem<T>::dCos );
            y.make(2,0);
            y[1] = 1;
            y[2] = 0;
            pb.count = 0;
            h1 = L/n;
            
            odeint.start(2);
            {
                const string  fn =  vformat("%s-cos-%s.dat", rk, id );
                ios::ocstream fp(fn,false);
                fp("%g %g %g\n", double(0), double(y[1]), double(y[2]));
                
                for(size_t i=0;i<n;++i)
                {
                    const T x0 = (i*L)/n;
                    const T x1 = ((i+1)*L)/n;
                    odeint(EqCos,y,x0,x1,h1,NULL);
                    fp("%g %g %g\n", double(x1), double(y[1]), double(y[2]));
                }
                std::cerr << "count=" << pb.count << std::endl;
            }
            
            
        }
        
        std::cerr << "LV" << std::endl;
        {
            equation EqLV( &pb, & problem<T>::dLV );
            y.make(2,0);
            pb.count = 0;
            h1 = L/n;
          
            y[1] = 1;
            y[2] = 2;
            
            odeint.start(2);
            {
                const string  fn =  vformat("%s-lv-%s.dat", rk, id );
                ios::ocstream fp(fn,false);
                fp("%g %g %g\n", double(0), double(y[1]), double(y[2]));
                
                for(size_t i=0;i<n;++i)
                {
                    const T x0 = (i*L)/n;
                    const T x1 = ((i+1)*L)/n;
                    odeint(EqLV,y,x0,x1,h1,NULL);
                    fp("%g %g %g\n", double(x1), double(y[1]), double(y[2]));
                }
                std::cerr << "count=" << pb.count << std::endl;
            }
            
            
        }

        
    }
}



YOCTO_UNIT_TEST_IMPL(ode_expl)
{
    
    solve<double,ode::driverCK<double>::type>( "ck" );
    solve<double,ode::driverDP<double>::type>( "dp" );
    solve<float,ode::driverCK<float>::type>( "ck" );
    solve<float,ode::driverDP<float>::type>( "dp" );
    
}
YOCTO_UNIT_TEST_DONE()

