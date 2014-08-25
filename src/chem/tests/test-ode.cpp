#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/lua/io.hpp"

using namespace yocto;
using namespace chemical;

namespace
{
    
    class Cell
    {
    public:
        Lua::State  VM;
        lua_State  *L;
        
        Cell( const string &filename ) :
        VM(),
        L( Lua::Config::DoFile(VM(),filename) )
        {
        }
        
        ~Cell() throw()
        {
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
    };
    
    
}

YOCTO_UNIT_TEST_IMPL(ode)
{
    if(argc<=1)
    {
        throw exception("need a filename");
    }
    Cell cell(argv[1]);
    
}
YOCTO_UNIT_TEST_DONE()
