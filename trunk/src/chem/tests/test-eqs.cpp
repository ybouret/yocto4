#include "yocto/utest/run.hpp"
#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(eqs)
{
    library    lib;
    equilibria eqs;
    
    Lua::State VM;
    lua_State *L = VM();
    
    
    if(argc>1)
    {
        Lua::Config::DoFile(L,argv[1]);
    }
    
    
    __lua::load(L, lib, "species");
    std::cerr << "lib=" << lib << std::endl;
    
    __lua::load(L,eqs,"eqs", lib);
    std::cerr << eqs << std::endl;
    
    eqs.compile_for(lib);
    std::cerr << "Nu=" << eqs.Nu << std::endl;

    for(size_t i=1;i<=eqs.M;++i)
    {
        eqs.C[i] = alea<double>() - 0.5;
        //if(i>2) eqs.C[i] = 0;
    }
    std::cerr << "C=" << eqs.C << std::endl;
    
    if(eqs.balance(eqs.C))
    {
        std::cerr << "Balanced=" << eqs.C << std::endl;
    }
    else
    {
        std::cerr << "Unable to balance" << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
