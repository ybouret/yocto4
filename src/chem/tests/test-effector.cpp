#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(effector)
{
    
    collection  lib;
    Lua::State  VM;
    lua_State  *L = VM();
    
    if(argc>1)
    {
        Lua::Config::DoFile(L, argv[1]);
    }
    
    _lua::load(L,lib,"species",0);
    std::cerr << "species="<< lib << std::endl;
    
    solution S(lib);
    for(size_t i=1;i<=S.size();++i) S[i] = alea<double>();
    std::cerr << "S="<< S << std::endl;
    
    effectors edb;
    _lua::load(L, edb, "eff1", lib);
    
    std::cerr << "Effectors: " << std::endl;
    vector<double> S_out(S.size(),0.0);
    solution dSdt(lib);
    
    for(effectors::iterator i=edb.begin();i!=edb.end();++i)
    {
        const effector &eff = **i;
        std::cerr << eff.name << std::endl;
        dSdt.ldz();
        for(size_t j=S.size();j>0;--j)
        {
            dSdt[j] = j;
        }
        
        eff.call(dSdt, 0.0, 0.001, S, S_out, lib);
        std::cerr << "dSdt=" << dSdt << std::endl;
    }
    
    edb.compute_rate(dSdt, 0.0, 0.001, S, S_out, lib);
    std::cerr << "rate=" << dSdt << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()
