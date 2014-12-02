#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/solution.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(sol)
{
    library    lib;
    equilibria eqs;
    
    Lua::State VM;
    lua_State *L = VM();
    
    
    if(argc>1)
    {
        Lua::Config::DoFile(L,argv[1]);
    }
    
    const char *ini_name = "ini";
    if(argc>2)
    {
        ini_name = argv[2];
    }
    
    __lua::load(L, lib, "species");
    std::cerr << "lib=" << lib << std::endl;
    
    __lua::load(L,eqs,"eqs", lib);
    std::cerr << eqs << std::endl;
    
    eqs.compile_for(lib);
    std::cerr << "Nu=" << eqs.Nu << std::endl;
    
    solution S(lib);
    
    {
        boot loader;
        
        loader.electroneutrality(lib);
        __lua::load(L,loader, ini_name, lib);
        
        std::cerr << loader << std::endl;
        
        eqs.load(loader,0.0);
        S.load(eqs.C);
        std::cerr << "S=" << S << std::endl;
        if(S.has("H+"))
            std::cerr << "pH=" << S.pH() << std::endl;
    }
    
    lua_settop(L,0);
    __lua::push(L,S);
    
    vector_t C(S.size(),0);
    __lua::load(L,C,lib);
    
    std::cerr << "C=" << C << std::endl;
    std::cerr << "S=" << S << std::endl;
    
    for(size_t i=C.size();i>0;--i)
    {
        C[i] = eqs.ran() - 0.5;
    }
    
    std::cerr << "rho0=" << C << std::endl;
    eqs.absorb(0.0, C, S);
    
    std::cerr << "rho1=" << C << std::endl;

}
YOCTO_UNIT_TEST_DONE()
