#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/lua/io.hpp"

using namespace yocto;
using namespace chemical;

namespace
{
    
    class Cell
    {
    public:
        Lua::State       VM;
        lua_State       *L;
        collection       lib;
        equilibria       eqs;
        effectors        edb;
        vector<solution> sol;
        const size_t     M;
        const size_t     iZ;
        const size_t     iV;
        const size_t     nvar;
        
        Cell( const string &filename ) :
        VM(),
        L( Lua::Config::DoFile(VM(),filename) ),
        lib(),
        eqs(),
        edb(),
        sol(4,as_capacity),
        M(0),
        iZ(0),
        iV(0),
        nvar(0)
        {
            _lua::load(L,lib,"species");
            std::cerr << "lib=" << lib << std::endl;
            size_t   count = lib.size();
            (size_t&)M    = count;
            (size_t&)iZ   = ++count;
            (size_t&)iV   = ++count;
            (size_t&)nvar = count;
            
            _lua::load(L,lib, eqs, "eqs");
            std::cerr << "eqs="<< std::endl << eqs << std::endl;
            
            prepare("inside");
            std::cerr << "inside=" << sol.back() << std::endl;
            prepare("outside");
            std::cerr << "outside=" << sol.back() << std::endl;
            
            {
                solution tmp(lib);
                sol.push_back(tmp);
            }
            
            _lua::load(L,edb,"eff",lib);
        }
        
        ~Cell() throw()
        {
        }
        
        void rates( array<double> &dYdt, double t, const array<double> &Y )
        {
            solution &S = sol[3];
            S.load(Y);
            const double zeta = Y[iZ];
            
            
            
            //compute chemical rates
            edb.compute_rate(dYdt, t, zeta, S, sol[2],lib);
            
            // append other
            
            
            //modify rates
            //std::cerr << "rates0="<< dYdt << std::endl;
            eqs.absorb(t, dYdt, Y);
            //std::cerr << "rates1="<< dYdt << std::endl;
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
        void prepare( const string &bootname)
        {
            boot loader;
            loader.electroneutrality(lib);
            _lua::load(L,lib,loader,bootname);
            std::cerr << loader << std::endl;
            solution tmp(lib);
            loader(tmp,lib,eqs,0.0);
            sol.push_back(tmp);
        }
    };
    
    
}

YOCTO_UNIT_TEST_IMPL(ode)
{
    if(argc<=1)
    {
        throw exception("need a filename");
    }
    Cell cell(argv[1]);
    
    vector<double> C(cell.nvar,0);
    cell.sol[1].save(C);
    std::cerr << "C="<< C << std::endl;
    diff_solver odeint(1e-7);
    odeint.start(cell.nvar);
    
    double h1=1e-4;
    diff_equation Eq( &cell, & Cell::rates );
    
    const double dt = 1e-2;
    double t = 0;
    while( t <= 1.0 )
    {
        const double t_next = t+dt;
        odeint(Eq,C,t,t_next,h1,&cell.eqs.callback);
        t = t_next;
        std::cerr << "C="<< C << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
