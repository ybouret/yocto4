#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/core/tao.hpp"

#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace chemical;
using namespace math;

namespace {


    class ChemSys
    {
    public:
        static const char  *p_names[];
        static const char  *p_loads[];
        static const size_t p_num;

        __lua::Library    lib;
        __lua::Equilibria eqs;
        __lua::Effectors  edb;
        variables         params;
        const size_t      nvar;
        vector<double>    C0;
        vector<double>    Cout;
        diff_equation     diffeq;
        diff_solver       odeint;
        double            diff_h;
        vector<double>    C;

        explicit ChemSys( lua_State *L ) :
        lib( L, "species"),
        eqs( L, "eqs", lib),
        edb( L, "eff" ),
        params(lib,p_names,p_loads,p_num),
        nvar( params.count ),
        C0(nvar,0),
        Cout(nvar,0),
        diffeq(this, & ChemSys::rate ),
        odeint( Lua::Config::Get<lua_Number>(L,"ftol")),
        diff_h(1e-6),
        C(nvar,0)
        {
            odeint.start(nvar);

            {
                boot loader;
                __lua::load(L, loader, "ini",lib);
                eqs.create(C0, loader,0.0);
                lib.display(std::cerr << "C_in=",C0) << std::endl;
                std::cerr << "charge_in=" << lib.charge(C0) << std::endl;
            }

            {
                boot loader;
                __lua::load(L, loader, "out",lib);
                eqs.create(Cout, loader,0.0);
                lib.display(std::cerr << "C_out=",Cout) << std::endl;
                std::cerr << "charge_out=" << lib.charge(C0) << std::endl;
            }

        }

        virtual ~ChemSys() throw()
        {
        }

        void rate( array<double> &dYdt, double t, const array<double> &Y )
        {
            // get chemical rated
            edb.rate(dYdt, t, Y, Cout, params);
            eqs.absorb(t,dYdt,Y);

        }

        void reset( double zeta )
        {
            tao::set(C,C0);
            diff_h = 1e-6;
        }


        void init_file(const string &filename) const
        {
            ios::ocstream fp(filename,false);
            fp("#t");
            for( library::const_iterator i=lib.begin();i!=lib.end();++i)
            {
                fp << " [" << (*i)->name << "]";
            }
            for( variables::iterator i=params.begin();i!=params.end();++i)
            {
                fp << " " << (**i).name;
            }
            fp("\n");
        }

        void save_file(const string &filename, const double t) const
        {
            ios::ocstream fp(filename,true);
            fp("%g",t);
            for(size_t i=1;i<=nvar;++i)
            {
                fp(" %g",C[i]);
            }
            fp("\n");
        }

        void process(const string &filename, const double tmax, double dt)
        {
            reset(0);
            init_file(filename);
            double t = 0;
            save_file(filename,t);

            while(t<tmax)
            {
                const double t1 = t + dt;
                odeint(diffeq,C,t,t1,diff_h,&eqs.callback);

                t = t1;

                save_file(filename,t);
            }
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(ChemSys);
    };

    const char * ChemSys:: p_names[] = { "zeta",  "V"  };
    const char * ChemSys:: p_loads[] = { "zeta0", "volume" };
    const size_t ChemSys:: p_num     = sizeof(ChemSys:: p_names)/sizeof(ChemSys:: p_names[0]);

}

YOCTO_UNIT_TEST_IMPL(ode)
{

    {
        ios::ocstream fp("psi.dat",false);
        for(double zeta=-5.0;zeta<=5.0;zeta+=0.01)
        {
            fp("%g %g\n", zeta, GHK_Psi(zeta) );
        }

    }

    library    lib;
    equilibria eqs;

    Lua::State VM;
    lua_State *L = VM();

    __lua::register_functions(L);

    if(argc>1)
    {
        Lua::Config::DoFile(L,argv[1]);
    }


    ChemSys       cs(L);

    cs.process("ode-raw.dat", 1, 1e-3);
    
    
    
}
YOCTO_UNIT_TEST_DONE()

