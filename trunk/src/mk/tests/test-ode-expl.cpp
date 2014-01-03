#include "yocto/utest/run.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"
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
    void solve()
    {
        typename ode::driverCK<T>::type odeCK(1e-4);
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
        T            h1 = L/1000;
        
        std::cerr << "Exp" << std::endl;
        {
            equation EqExp( &pb, & problem<T>::dExp  );
            callback OnExp( &pb, & problem<T>::ExpCB );
            
            y.make(1,0);
            y[1] = 1;
            odeCK.start(1);
            pb.count = 0;
            
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
        
        
        std::cerr << "Cos" << std::endl;
        {
            equation EqCos( &pb, & problem<T>::dCos );
            y.make(2,0);
            y[1] = 1;
            y[2] = 0;
            odeCK.start(2);
            pb.count = 0;

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
        
    }
}



YOCTO_UNIT_TEST_IMPL(ode_expl)
{
    
    solve<double>();
    
    
    
}
YOCTO_UNIT_TEST_DONE()

