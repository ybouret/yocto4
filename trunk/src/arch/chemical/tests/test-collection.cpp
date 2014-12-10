#include "yocto/chemical/collection.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

#include "yocto/chemical/lua/io.hpp"

using namespace yocto;
using namespace chemical;

static inline
void ioCallback(lua_State *L, species &sp )
{
    std::cerr << "Parsing data for " << sp.name << std::endl;
    std::cerr << "\t=> " << lua_typename(L, lua_type(L,-1)) << std::endl;
}

YOCTO_UNIT_TEST_IMPL(collection)
{
    
    collection lib;
    if(argc<=1)
    {
        lib.add("H+",1);
        lib.add("HO-",-1);
    }
    else
    {
        Lua::State VM;
        lua_State *L = VM();
        Lua::Config::DoFile(L, argv[1]);
        species_ctor F( cfunctor2(ioCallback) );
        _lua::load(L,lib,"species",&F);
    }
    
    std::cerr << lib << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
