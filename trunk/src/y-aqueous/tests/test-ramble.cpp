#include "yocto/utest/run.hpp"
#include "yocto/aqueous/ramble.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/spade/array1d.hpp"

using namespace yocto;
using namespace aqueous;
using namespace spade;

namespace
{
    typedef array1D<double>            Array;
    typedef ramble::of<double,int,int> ramble_type;
    
    typedef ramble_type::library Library;
}

const char code[] =
"species="
"{"
"{ \"H+\", 1 , { 2e-8, 5} },"
"{ \"HO-\", -1 , { 1e-8} }"
"}"
"weak = {"
"{ \"water\", 1e-14, { 1, \"H+\"}, { 1, \"HO-\" } }"
"}"
"ftol=1e-5"
;

YOCTO_UNIT_TEST_IMPL(ramble)
{
    
    Lua::State  VM;
    lua_State  *L = VM();
    Lua::Config::DoString(L, code);
    Library lib( "species", L );
    ramble::chemsys cs( lib, "weak", L, "ftol" );
    
    
}
YOCTO_UNIT_TEST_DONE()

