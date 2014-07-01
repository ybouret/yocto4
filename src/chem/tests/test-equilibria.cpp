#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(equilibria)
{
    collection  lib;
    equilibria  cs;
    Lua::State  VM;
    lua_State  *L = VM();
    
    if(argc>1)
    {
        Lua::Config::DoFile(L, argv[1]);
    }
    
    _lua::load(L,lib,"species",0);
    std::cerr << lib << std::endl;
    
    _lua::load(L,lib,cs,"eqs");
    std::cerr << cs << std::endl;
    
    cs.startup(lib);
    solution S(lib);
    for(size_t i=1;i<=S.size();++i) S[i] = i * 1e-5;
    
    std::cerr << "S=" << S << std::endl;

    cs.computeGamma(0,S);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    
    cs.computeGammaAndPhi(0,S);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "Phi="   << cs.Phi   << std::endl;
    
    if( cs.computeNewtonStep() )
    {
        std::cerr << "xi=" << cs.xi << std::endl;
        std::cerr << "dC=" << cs.dC << std::endl;
    }
    else
    {
        std::cerr << "singular" << std::endl;
    }
    
    S[3] = S[4] = 0;
    S[3] = 0.1;
    if( cs.normalize(0.0, S))
    {
        
    }
    

}
YOCTO_UNIT_TEST_DONE()
