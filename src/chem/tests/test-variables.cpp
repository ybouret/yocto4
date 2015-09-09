#include "yocto/utest/run.hpp"

#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(variables)
{
    library    lib;
    equilibria eqs;

    Lua::State VM;
    lua_State *L = VM();
    __lua::register_functions(L);
    
    if(argc>1)
    {
        Lua::Config::DoFile(L,argv[1]);
    }

    __lua::load(L, lib, "species");
    std::cerr << "lib=" << lib << std::endl;

    const char *pname[] = { "zeta",  "V"      };
    const char *pload[] = { "zeta0", "volume" };
    variables   params(lib,pname,pload,sizeof(pname)/sizeof(pname[0]));

    std::cerr << "params=" << params << std::endl;

    __lua::load(L,eqs,"eqs", lib);
    std::cerr << eqs << std::endl;

    eqs.compile_for(lib);
    std::cerr << "Nu=" << eqs.Nu << std::endl;

    vector_t S(lib.size(),0);

    {
        boot loader;

        loader.electroneutrality(lib);
        __lua::load(L,loader, "ini", lib);

        std::cerr << loader << std::endl;

        eqs.create(S,loader,0.0);

        std::cerr << "S=" << S << std::endl;
        if(lib.has("H+"))
            std::cerr << "pH=" << lib.pH(S) << std::endl;
    }

}
YOCTO_UNIT_TEST_DONE()
