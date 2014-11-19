#include "yocto/utest/run.hpp"
#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(nrm)
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
    
    const size_t M = eqs.M;
    vector<double> C0(M,0);
    
    for(size_t i=1;i<=M;++i)
    {
        C0[i] = alea<double>() - 0.1;
        //if(i>2) eqs.C[i] = 0;
    }
    std::cerr << "C0=" << C0 << std::endl;
    
    if(eqs.balance(C0))
    {
        std::cerr << "Balanced=" << C0 << std::endl;
        if( eqs.normalize(C0, 0, true) )
        {
            std::cerr << "Normalized=" << C0 << std::endl;
        }
        else
        {
            std::cerr << "Not Normalized" << std::endl;
        }
    }
    else
    {
        std::cerr << "Not Balanced" << std::endl;
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
