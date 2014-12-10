#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/lua/io.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(volume)
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
    
    boot loader;
    loader.electroneutrality(lib);
    _lua::load(L,lib,loader,"ini");
    
    std::cerr << loader << std::endl;
    
    solution S(lib);
    loader(S,lib,cs,0.0);
    std::cerr << "S=" << S << std::endl;
    
    solution S1(lib);
    
    ios::ocstream fp("dilution.dat",false);
    for(double fac=0.01;fac<=2;fac+=0.01)
    {
        for(size_t i=S1.size();i>0;--i)
        {
            S1[i] = S[i] * fac;
        }
        cs.normalize(0, S1);
        fp("%g %g\n", fac, S1.pH());
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()

