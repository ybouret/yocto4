#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/code/rand.hpp"

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
    
    
    S[3] = S[4] = 0;
    S[3] = 0.01;
    std::cerr << "S=" << S << std::endl;
    std::cerr << "Normalizing..." << std::endl;
    if( cs.normalize(0.0, S))
    {
        std::cerr << "S=" << S << std::endl;
    }
    
    solution dS(S);
    for(size_t i=dS.size();i>0;--i)
    {
        dS[i] = (1e-4*i)*(alea<double>()-0.5);
    }
    std::cerr << "dS0=" << dS << std::endl;
    cs.absorb(0.0, dS, S);
    std::cerr << "dS1=" << dS << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
