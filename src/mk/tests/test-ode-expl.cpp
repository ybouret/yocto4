#include "yocto/utest/run.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"
#include "yocto/math/ode/explicit/driver-dp.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

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
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(problem);
    };
    
    
    template <typename T>
    static inline void solve()
    {
        const T eps = 1e-7;
        typename ode::driverCK<T>::type odeCK(eps);
        typename ode::driverDP<T>::type odeDP(eps);
        
        problem<T>                      pb;
        typedef typename ode::field<T>::explicit_type equation;
        typedef typename ode::field<T>::callback_type callback;
        const char *id = typeid(T).name();
        
        std::cerr << "--------------------" << std::endl;
        std::cerr << "solving <" << id << ">" << std::endl;
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
            odeCK.start(1);
            odeDP.start(1);
            std::cerr << "\tCK" << std::endl;
            {
                const string  fn =  vformat("ck-exp-%s.dat",  id );
                ios::ocstream fp(fn,false);
                fp("%g %g\n", double(0), double(y[1]));
                
                for(size_t i=0;i<n;++i)
                {
                    const T x0 = (i*L)/n;
                    const T x1 = ((i+1)*L)/n;
                    odeCK(EqExp,y,x0,x1,h1,&OnExp);
                    fp("%g %g\n", double(x1), double(y[1]));
                }
                
                std::cerr << "count=" << pb.count << std::endl;
            }
            
            y[1]     = 1;
            pb.count = 0;
            h1 = L/n;
            std::cerr << "\tDP" << std::endl;
            {
                const string  fn =  vformat("dp-exp-%s.dat",  id );
                ios::ocstream fp(fn,false);
                fp("%g %g\n", double(0), double(y[1]));
                
                for(size_t i=0;i<n;++i)
                {
                    const T x0 = (i*L)/n;
                    const T x1 = ((i+1)*L)/n;
                    odeDP(EqExp,y,x0,x1,h1,&OnExp);
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
            
            odeCK.start(2);
            odeDP.start(2);
            std::cerr << "\tCK" << std::endl;
            {
                const string  fn =  vformat("ck-cos-%s.dat", id );
                ios::ocstream fp(fn,false);
                fp("%g %g %g\n", double(0), double(y[1]), double(y[2]));
                
                for(size_t i=0;i<n;++i)
                {
                    const T x0 = (i*L)/n;
                    const T x1 = ((i+1)*L)/n;
                    odeCK(EqCos,y,x0,x1,h1,NULL);
                    fp("%g %g %g\n", double(x1), double(y[1]), double(y[2]));
                }
                std::cerr << "count=" << pb.count << std::endl;
            }
            
            y[1] = 1;
            y[2] = 0;
            pb.count = 0;
            h1 = L/n;
            
            std::cerr << "\tDP" << std::endl;
            {
                const string  fn =  vformat("dp-cos-%s.dat", id );
                ios::ocstream fp(fn,false);
                fp("%g %g %g\n", double(0), double(y[1]), double(y[2]));
                
                for(size_t i=0;i<n;++i)
                {
                    const T x0 = (i*L)/n;
                    const T x1 = ((i+1)*L)/n;
                    odeDP(EqCos,y,x0,x1,h1,NULL);
                    fp("%g %g %g\n", double(x1), double(y[1]), double(y[2]));
                }
                std::cerr << "count=" << pb.count << std::endl;
            }
            
        }
        
    }
}



YOCTO_UNIT_TEST_IMPL(ode_expl)
{
    
    solve<double>();
    solve<float>();
    
    
}
YOCTO_UNIT_TEST_DONE()

