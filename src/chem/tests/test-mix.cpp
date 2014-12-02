#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/solution.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(mix)
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
    
    vector<solution> solutions(2,as_capacity);
    
    {
        boot loader;
        
        loader.electroneutrality(lib);
        __lua::load(L,loader, "ini1", lib);
        
        std::cerr << loader << std::endl;
        
        eqs.load(loader,0.0);
        solution S(lib);
        S.load(eqs.C);
        std::cerr << "S1=" << S << std::endl;
        std::cerr << "pH1=" << S.pH() << std::endl;
        solutions.push_back(S);
    }
    
    {
        boot loader;
        
        loader.electroneutrality(lib);
        __lua::load(L,loader, "ini2", lib);
        
        std::cerr << loader << std::endl;
        
        eqs.load(loader,0.0);
        solution S(lib);
        S.load(eqs.C);
        std::cerr << "S2="  << S      << std::endl;
        std::cerr << "pH2=" << S.pH() << std::endl;
        solutions.push_back(S);
    }
    

    vector<double> alpha(1000,0);
    for(size_t i=alpha.size();i>0;--i)
    {
        alpha[i] = alea<double>();
    }
    quicksort(alpha);
    alpha[1]              = 0;
    alpha[ alpha.size() ] = 1;
    
    
    vector<double> weights(2,0);
    solution       Sol(lib);
    ios::ocstream  fp("mix.dat",false);
    
    for(size_t i=1;i<=alpha.size();++i)
    {
        weights[1] = alpha[i];
        weights[2] = 1.0 - alpha[i];
        eqs.mix(Sol, solutions, weights, 0);
        fp("%g %g\n", alpha[i], Sol.pH());
        std::cerr << ".";
        std::cerr.flush();
    }
    std::cerr << std::endl;
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
