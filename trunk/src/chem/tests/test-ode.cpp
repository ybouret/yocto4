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
        Lua::State       VM;
        lua_State       *L;
        collection       lib;
        equilibria       eqs;
        vector<solution> sol;
        vector<double>   C;
        
        Cell( const string &filename ) :
        VM(),
        L( Lua::Config::DoFile(VM(),filename) ),
        lib(),
        eqs(),
        sol(4,as_capacity),
        C()
        {
            _lua::load(L,lib,"species");
            std::cerr << "lib=" << lib << std::endl;
            _lua::load(L,lib, eqs, "eqs");
            std::cerr << "eqs="<< std::endl << eqs << std::endl;
            
            prepare("inside");
            std::cerr << "inside=" << sol.back() << std::endl;
            prepare("outside");
            std::cerr << "outside=" << sol.back() << std::endl;
            
            C.make(lib.size()+2,0.0);
            sol[1].save(C);
        }
        
        ~Cell() throw()
        {
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
        void prepare( const string &bootname)
        {
            boot loader;
            loader.electroneutrality(lib);
            _lua::load(L,lib,loader,bootname);
            std::cerr << loader << std::endl;
            solution tmp(lib);
            loader(tmp,lib,eqs,0.0);
            sol.push_back(tmp);
        }
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
