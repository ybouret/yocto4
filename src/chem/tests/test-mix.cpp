#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/lua/io.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(mix)
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
    
    vector<solution> sols;
    for(unsigned i=1;i<=2;++i)
    {
        solution S(lib);
        boot loader;
        loader.electroneutrality(lib);
        const string loader_id = vformat("ini%u",i);
        _lua::load(L,lib, loader,loader_id);
        loader(S,lib,cs,0.0);
        sols.push_back(S);
    }

    const size_t nw = sols.size();
    for(size_t i=1;i<=nw;++i)
    {
        std::cerr << "S" << i << "=" << sols[i] << std::endl;
    }
    
    
    vector<double> weights(nw,1.0);
    solution S(lib);
    const size_t NMAX = 200;
    ios::ocstream fp("mix.dat",false);
    for(size_t i=0;i<=NMAX;++i)
    {
        weights[2] = double(i)/NMAX;
        weights[1] = double(NMAX-i)/NMAX;
        cs.mix(S,sols,weights,0.0);
        std::cerr << "pH=" << S.pH() << std::endl;
        fp("%g %g\n",weights[2],S.pH());
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

