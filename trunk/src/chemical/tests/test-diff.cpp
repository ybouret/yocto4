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
    typedef ode::Field<double>::Callback ODE_CB;

    typedef algebra<double> mkl;
    
    class ChemDiff : public ODE_Solver
    {
    public:
        chemical::equilibria &cs;
        chemical::effectors  &eff;
        chemical::solution    S;
        chemical::solution    dSdt;
        chemical::solution    S_out;
        const size_t          nv;
        vector<double>        yy;
        ODE                   eq;
        ODE_CB                cb;
        size_t                calls;
        
#if defined(_MSC_VER)
		// 'this' in constructor
#pragma warning ( disable : 4355 )
#endif
        explicit ChemDiff(chemical::collection   &lib,
                          chemical::equilibria   &user_cs,
                          chemical::effectors    &user_eff,
                          chemical::boot::loader &ini
                          ) : ODE_Solver(1e-7),
        cs(user_cs),
        eff(user_eff),
        S(lib),
        dSdt(lib),
        S_out(lib),
        nv(lib.size()),
        yy(nv,0),
        eq( this, &ChemDiff::Compute ),
        cb( this, &ChemDiff::Normalize ),
        calls(0)
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
            dSdt.ldz();
            eff.append(dSdt, t, 0.0, S, S_out);
            cs.load_C(y);
            dSdt.save(cs.dC);
            cs.legalize_dC(t);
            
            for(size_t i=nv;i>0;--i)
                dydt[i] = cs.dC[i];
            
            ++calls;
        }
        
        void Normalize( array<double> &y, double t )
        {
            cs.load_C(y);
            cs.normalize_C(t);
            cs.save_C(y);
        }
        
        // start from S => compute new S
        void update( double t, double t1, double &dt1 )
        {
            ODE_Solver &odeint = *this;
            S.save(yy);
            odeint(eq,yy,t,t1,dt1,NULL);
            cs.load_C(y);
            cs.normalize_C(t1);
            S.load(cs.C);
        }
        
        void update_cb( double t, double t1, double &dt1 )
        {
            ODE_Solver &odeint = *this;
            S.save(yy);
            odeint(eq,yy,t,t1,dt1,&cb);
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
    
    chemical::boot::loader ini;
    chemical:: _lua::load(L,ini,"ini",lib);
    ini.electroneutrality(lib);
    
    std::cerr << ini << std::endl;
    
    
    chemical::effectors eff;
    chemical::_lua::load(L, eff, "effectors");
    std::cerr << "Effectors: " << std::endl << eff << std::endl;
    chemical::solution s(lib);
    chemical::solution ds(lib);
    
    eff["NaOH"].factor = 1e-3;
    eff.append(ds, 0, 0, s, s);
    
    std::cerr << "ds=" << ds << std::endl;
    
    std::cerr << "-- Initializing ChemDiff" << std::endl;
    
    ChemDiff cd1(lib,cs,eff,ini);
    ChemDiff cd2(lib,cs,eff,ini);

    
    
    const double dt   = 0.01;
    double       dt1  = dt/10;
    double       dt2  = dt1;
    double       t    = 0;
    
    ios::ocstream fp("diff.dat",false);
    fp("%g %g\n", t, cd1.S.pH());
    for(size_t i=0;;++i)
    {
        const double t1 = t + dt;
        cd1.update(t,t1,dt1);
        cd2.update_cb(t,t1,dt2);
        t = t1;
        std::cerr << "pH=" << cd1.S.pH() << std::endl;
        fp("%g %g %g\n", t, cd1.S.pH(), cd2.S.pH() );
        if( t > 2 ) break;
    }
    
    std::cerr << "cd1 #calls=" << cd1.calls << std::endl;
    std::cerr << "cd2 #calls=" << cd2.calls << std::endl;

    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
