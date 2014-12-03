#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace chemical;

namespace {
    
    
    class ChemSys
    {
    public:
        static const char  *p_names[];
        static const size_t p_count;
        
        __lua::Library    lib;
        __lua::Equilibria eqs;
        __lua::Effectors  edb;
        parameters        params;
        const size_t      nvar;
        vector<double>    C0;
        vector<double>    Cout;
        
        explicit ChemSys( lua_State *L ) :
        lib( L, "species"),
        eqs( L, "eqs", lib),
        edb( L, "eff" ),
        params(lib,p_names,p_count),
        nvar( lib.size() + params.count ),
        C0(nvar,0),
        Cout(nvar,0)
        {
            
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
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(ChemSys);
    };
    
    const char * ChemSys:: p_names[] = { "zeta", "V"  };
    const size_t ChemSys:: p_count   = sizeof(ChemSys:: p_names)/sizeof(ChemSys:: p_names[0]);
    
}

YOCTO_UNIT_TEST_IMPL(ode)
{
    library    lib;
    equilibria eqs;
    
    Lua::State VM;
    lua_State *L = VM();
    
    
    if(argc>1)
    {
        Lua::Config::DoFile(L,argv[1]);
    }
    
    
    ChemSys cs(L);
    
    
    
}
YOCTO_UNIT_TEST_DONE()