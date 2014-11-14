#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace chemical;

namespace
{
    void display_species( lua_State *L, species &sp )
    {
        std::cerr << "extra data for " << sp.name << std::endl;
        std::cerr << "-- " << lua_typename(L, lua_type(L, -1) ) << std::endl;
    }
}

YOCTO_UNIT_TEST_IMPL(lib)
{
    library lib;
    
    Lua::State VM;
    lua_State *L = VM();
    
    
    if(argc>1)
    {
        Lua::Config::DoFile(L,argv[1]);
    }
    
    __lua::species_callback cb( cfunctor2(display_species) );
    
    __lua::load(L, lib, "species", &cb);
    std::cerr << "lib=" << lib << std::endl;
}
YOCTO_UNIT_TEST_DONE()
