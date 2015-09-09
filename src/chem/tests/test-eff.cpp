#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(eff)
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

#if 1
    const char *pname[] = { "zeta",  "V" };
    const char *pload[] = { "zeta0", "volume" };

    variables params(lib,pname,pload,sizeof(pname)/sizeof(pname[0]));
    std::cerr << "params=" << params << std::endl;

    const size_t nvar = params.count;
    vector_t S(nvar,0);
    
    {
        boot loader;
        
        __lua::load(L,loader, ini_name, lib);
        
        std::cerr << loader << std::endl;
        
        eqs.create(S,loader,0.0);
        
        std::cerr << "S=" << S << std::endl;
        if(lib.has("H+"))
            std::cerr << "pH=" << lib.pH(S) << std::endl;
    }
    
    
    
    effectors edb;
    edb.reserve(4);
    
    __lua::load(L,edb,"eff");
    
    vector_t Sout(S);
    
    vector_t rho(eqs.M,0);
    
    S[ params["zeta"] ] = -0.05;
    
    edb.rate(rho, 0.0, S, Sout, params);
    std::cerr << "rho=" << rho << std::endl;
    edb["Soude"].pace = 2.0;
#endif
    
}
YOCTO_UNIT_TEST_DONE()

