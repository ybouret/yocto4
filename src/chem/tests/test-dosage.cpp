#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/solution.hpp"

#include "yocto/fs/vfs.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(dosage)
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
    
    solution S0(lib);
    const double V0 = Lua::Config::Get<lua_Number>(L, "V0");
    {
        boot loader;
        
        loader.electroneutrality(lib);
        __lua::load(L,loader, "ini", lib);
        
        std::cerr << loader << std::endl;
        
        eqs.load(loader,0.0);
        S0.load(eqs.C);
        std::cerr << "S0=" << S0 << std::endl;
        std::cerr << "pH=" << S0.pH() << std::endl;
    }
    
    solution S1(lib);

    {
        boot loader;
        
        loader.electroneutrality(lib);
        __lua::load(L,loader, "dose", lib);
        
        std::cerr << loader << std::endl;
        
        eqs.load(loader,0.0);
        S1.load(eqs.C);
        std::cerr << "S1=" << S1 << std::endl;
        std::cerr << "pH=" << S1.pH() << std::endl;
    }
    
    const double V1 = Lua::Config::Get<lua_Number>(L, "V1");
    
    solution S(lib);
    eqs.computeK(0);
    std::cerr << "K=" << eqs.K << std::endl;
    const size_t STEPS=1000;
    
    
    string output = vfs::get_base_name(argv[1]);
    vfs::change_extension(output, "dat");
    
    ios::ocstream::overwrite(output);
    
    for(size_t i=0;i<=STEPS;++i)
    {
        const double Vadd = (i*V1)/STEPS;
        const double Vnew = V0 + (i*V1)/STEPS;
        for(size_t j=S.size();j>0;--j)
        {
            S[j] = (S0[j] * V0 + S1[j] * Vadd)/Vnew;
        }
        //std::cerr << "S[" << i <<"]=" << S << std::endl;
        if(!eqs.balance(S))
        {
            throw exception("Cannot balance solution!");
        }
        if(!eqs.normalize(S, -1, false))
        {
            throw exception("Cannot normalize solution!");
        }
        ios::ocstream fp(output,true);
        fp("%g %g\n", Vadd, S.pH());
        std::cerr << "."; std::cerr.flush();
    }
    
    std::cerr << std::endl << "saved in " << output << std::endl;
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()