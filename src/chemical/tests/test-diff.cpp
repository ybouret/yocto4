#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/chemical/solution.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"
#include "yocto/math/kernel/algebra.hpp"

using namespace yocto;
using namespace math;

namespace
{
    typedef ode::driverCK<double>::type  ODE_Solver;
    typedef ode::Field<double>::Equation ODE;
    typedef algebra<double> mkl;
    
    class ChemDiff : public ODE_Solver
    {
    public:
        chemical::equilibria &cs;
        chemical::effectors  &eff;
        chemical::solution    S;
        chemical::solution    dSdt;
        const size_t          nv;
        vector<double>        yy;
        ODE                   eq;
        
        explicit ChemDiff(chemical::collection &lib,
                          chemical::equilibria &user_cs,
                          chemical::effectors  &user_eff,
                          chemical::initializer &ini
                          ) : ODE_Solver(1e-5),
        cs(user_cs),
        eff(user_eff),
        S(lib),
        dSdt(lib),
        nv(lib.size()),
        yy(nv,0),
        eq( this, &ChemDiff::Compute )
        {
            if(nv<=0) throw exception("No Species");
            ini(cs,lib,0.0);
            start(nv);
            S.load(cs.C);
            std::cerr << S << std::endl;
            std::cerr << "pH=" << S.pH() << std::endl;
        }
        
        virtual ~ChemDiff() throw() {}
        
        void Compute( array<double> &dydt, double t, const array<double> &y )
        {
            S.load(y);
            eff.collect(dSdt, t, 0.0, S);
            for(size_t i=nv;i>0;--i) cs.C[i] = y[i];
            dSdt.save(cs.dC);
            cs.legalize_dC(t);
            for(size_t i=nv;i>0;--i)
                dydt[i] = cs.dC[i];
        }
        
        // start from S => compute new S
        void update( double t, double t1, double &dt1 )
        {
            ODE_Solver &odeint = *this;
            S.save(yy);
            odeint(eq,yy,t,t1,dt1,NULL);
            S.load(yy);
        }
        
        
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(ChemDiff);
    };
    
    
}


YOCTO_UNIT_TEST_IMPL(diff)
{
    Lua::State VM;
    lua_State *L = VM();
    
    if( argc >1 )
    {
        const string filename = argv[1];
        Lua::Config::DoFile(L, filename);
    }
    
    chemical::collection lib;
    chemical:: _lua:: load(L, lib, "species");
    
    std::cerr <<  lib << std::endl;
    
    chemical::equilibria cs;
    chemical:: _lua::load(L, lib, cs,"eqs");
    
    std::cerr  << cs << std::endl;
    
    chemical::initializer ini;
    chemical:: _lua::load(L,ini,"ini");
    ini.electroneutrality(lib);
    
    std::cerr << ini << std::endl;
    
    
    chemical::effectors eff;
    chemical::_lua::load(L, eff, "effectors");
    std::cerr << "Effectors: " << std::endl << eff << std::endl;
    chemical::solution s(lib);
    chemical::solution ds(lib);
    
    eff["NaOH"].factor = 1e-3;
    eff.collect(ds, 0, 0, s);
    
    std::cerr << "ds=" << ds << std::endl;
    
    std::cerr << "-- Initializing ChemDiff" << std::endl;
    
    ChemDiff chemdiff(lib,cs,eff,ini);
    ODE      Eq( &chemdiff, &ChemDiff::Compute);
    
    
    
    const double dt   = 0.01;
    double       dt1  = dt/10;
    double       t    = 0;
    
    ios::ocstream fp("diff.dat",false);
    fp("%g %g\n", t, chemdiff.S.pH());
    for(size_t i=0;;++i)
    {
        const double t1 = t + dt;
        chemdiff.update(t,t1,dt1);
        t = t1;
        std::cerr << "pH=" << chemdiff.S.pH() << std::endl;
        fp("%g %g\n", t, chemdiff.S.pH());
        if( t > 5 ) break;
    }
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
