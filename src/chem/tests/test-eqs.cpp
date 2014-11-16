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
    std::cerr << "Nu2=" << eqs.Nu2 << std::endl;
    std::cerr << "dNu2=" << eqs.dNu2 << std::endl;
    std::cerr << "ANu2=" << eqs.ANu2 << std::endl;
    std::cerr << "iNu2=" << eqs.iNu2 << std::endl;

    for(size_t i=1;i<=eqs.M;++i)
    {
        eqs.C[i] = alea<double>() - 0.5;
    }
    std::cerr << "C=" << eqs.C << std::endl;
    
    eqs.balance(eqs.C);
    
}
YOCTO_UNIT_TEST_DONE()
