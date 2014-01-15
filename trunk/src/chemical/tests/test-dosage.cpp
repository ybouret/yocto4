#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/chemical/solution.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/fs/vfs.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(dosage)
{
    //--------------------------------------------------------------------------
    // use lua to load parameters
    //--------------------------------------------------------------------------
    Lua::State VM;
    lua_State *L = VM();
    
    if( argc >1 )
    {
        const string filename = argv[1];
        Lua::Config::DoFile(L, filename);
    }
    else
        throw exception("need a config file to do something");
    
    //--------------------------------------------------------------------------
    // load the species into collection
    //--------------------------------------------------------------------------
    chemical::collection      lib;
    chemical:: _lua:: load(L, lib, "species");
    std::cerr << lib << std::endl;
    
    //--------------------------------------------------------------------------
    // load the reactions
    //--------------------------------------------------------------------------
    chemical::equilibria cs;
    chemical:: _lua::load(L, lib, cs,"eqs");
    std::cerr  << cs << std::endl;
    
    //--------------------------------------------------------------------------
    // prepare the initializer
    //--------------------------------------------------------------------------
    chemical::boot::loader ini;
    ini.electroneutrality(lib);
    chemical:: _lua::load(L,ini,"ini",lib);
    
    std::cerr << ini << std::endl;
    
    //--------------------------------------------------------------------------
    // find the initial composition
    //--------------------------------------------------------------------------
    //cs.ftol = 1e-7;
    ini(cs,lib,0.0);
    
    chemical::solution S0(lib);
    S0.load(cs.C);
    std::cerr << "S0=" << S0 << std::endl;
    std::cerr << "pH=" << S0.pH() << std::endl;
    const double V0 = Lua::Config::Get<lua_Number>(L,"V0");
    std::cerr << "starting with V0=" << V0 << std::endl;
    
    //--------------------------------------------------------------------------
    // find the titration
    //--------------------------------------------------------------------------
    vector<string> added;
    Lua::Config::GetTable<string>(L, "S1", added);
    const double   C1 = Lua::Config::Get<lua_Number>(L,"C1");
    chemical::solution S1(lib);
    for( size_t i=1; i<=added.size(); ++i)
    {
        S1[ added[i] ] = C1;
    }
    std::cerr << "dosage with " << S1 << std::endl;
    const double V1 = Lua::Config::Get<lua_Number>(L,"V1");
    std::cerr << "Adding up to V1=" << V1 << std::endl;
    
    //--------------------------------------------------------------------------
    // Here we go
    //--------------------------------------------------------------------------
    size_t       N    = 1000;
    const double dV   = V1/N;
    double       Vtot = V0;
    chemical::solution dn(S1);
    dn.mul(dV);
    
    string fn = vfs::get_base_name(argv[1]);
    vfs::change_extension(fn, "dat");
    std::cerr << "saving into " << fn << std::endl;
    
    {
        ios::ocstream fp(fn,false);
        fp << "#V pH";
        S0.write_header(fp);
        fp << "\n";
        
        fp << "0";
        if( S0["H+"]<=0 )
            throw exception("no H+");
        fp(" %.5e", S0.pH());
        S0.write_values(fp);
        fp << "\n";
    }
    
    for(size_t i=1; i<= N; ++i )
    {
        S0.mul(Vtot);        // S0 => matter quantities
        S0.add(dn);          // add solution
        Vtot += dV;          // increase volume
        S0.mul(1.0/Vtot);    // dissolution
        S0.save(cs.C);       // prepare equilibria
        
        if(!cs.normalize_C(0.0) )
        {
            std::cerr << "bad=" << cs.C << std::endl;
            throw exception("invalid composition");
        }
        S0.load(cs.C);
        
        ios::ocstream fp(fn,true);
        fp("%.5e", i*dV);
        if( S0["H+"]<=0 )
            throw exception("no H+");
        fp(" %.5e", S0.pH());
        S0.write_values(fp);
        fp << "\n";
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
