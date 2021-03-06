#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"
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
    
    vector<vector_t> solutions(2,as_capacity);
    
    {
        boot loader;
        
        __lua::load(L,loader, "ini1", lib);
        
        std::cerr << loader << std::endl;
        
        vector_t S(lib.size(),0);
        eqs.create(S,loader,0.0);
        std::cerr << "S1=" << S << std::endl;
        std::cerr << "pH1=" << lib.pH(S) << std::endl;
        solutions.push_back(S);
    }
    
    {
        boot loader;
        __lua::load(L,loader, "ini2", lib);
        
        std::cerr << loader << std::endl;
        
        vector_t S(lib.size(),0);
        eqs.create(S,loader,0.0);
        std::cerr << "S2="  << S      << std::endl;
        std::cerr << "pH2=" << lib.pH(S) << std::endl;
        solutions.push_back(S);
    }
    
    std::cerr << "Loading matrix of solutions..." << std::endl;
    {
        matrix_t solutions;
        __lua::load(L,solutions, "mix", lib, eqs, 0.0);
        std::cerr << "solutions=" << std::endl << solutions << std::endl;
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
    vector_t       Sol(lib.size(),0);
    ios::ocstream  fp("mix.dat",false);
    
    for(size_t i=1;i<=alpha.size();++i)
    {
        weights[1] = alpha[i];
        weights[2] = 1.0 - alpha[i];
        eqs.mix(Sol, solutions, weights, 0);
        fp("%g %g\n", alpha[i], lib.pH(Sol));
        std::cerr << ".";
        std::cerr.flush();
    }
    std::cerr << std::endl;


    
    
    
}
YOCTO_UNIT_TEST_DONE()
