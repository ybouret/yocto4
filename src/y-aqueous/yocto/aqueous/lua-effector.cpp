#include "yocto/aqueous/lua.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/lua/lua-maths.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
	namespace aqueous
	{
        
        _lua:: effector:: ~effector() throw()
        {
        }
        
        _lua:: effector:: effector( const string &id ) :
        name(id),
        input(),
        output()
        {
            
        }
        
        const string & _lua::effector:: key() const throw()
        {
            return name;
        }
        
        
        static inline void __load_strings(const char       *id,
                                          const char       *ctx,
                                          sequence<string> &seq, 
                                          lua_State        *L)
        {
            assert( lua_istable(L,-1) );
            const unsigned ns = lua_objlen(L, -1);
            for( unsigned i=1; i <= ns; ++i )
            {
                lua_rawgeti(L,-1,i);
                if( !lua_isstring(L, -1) )
                    throw exception("effector '%s', load <%s>: field #%u is not a string",id,ctx,i);
                size_t       len = 0;
                const char  *ptr = lua_tolstring(L, -1, &len);
                const string var(ptr,len);
                seq.push_back(var);
                lua_pop(L,1);
            }
        }
        
        void _lua:: load( lua_State *L, effector::db &effectors, const string &effname )
        {
            //==================================================================
            // load the effectors table
            //==================================================================
            lua_settop(L, 0);
            const char *tabname = effname.c_str();
            lua_getglobal(L, tabname);
            if( !lua_istable(L, -1) )
                throw exception("no effectors table '%s'", tabname );
            
            //==================================================================
            // count effectors
            //==================================================================
            const unsigned neff = lua_objlen(L, -1);
            std::cerr << "*** Parsing " << neff << " effector(s)" << std::endl;
            for( unsigned i=1; i <= neff; ++i )
            {
                //--------------------------------------------------------------
                lua_rawgeti(L, -1, i); //-- get effector table
                //--------------------------------------------------------------
                {
                    //----------------------------------------------------------
                    lua_rawgeti(L, -1, 1); //-- get effector name
                    //----------------------------------------------------------
                    if( !lua_isstring(L,-1) )
                        throw exception("int [%s]: effector #%u: name is not a string",tabname,i);
                    size_t      len = 0;
                    const char *ptr = lua_tolstring(L, -1, &len);
                    const string name(ptr,len);
                    const char  *id = name.c_str();
                    
                    //----------------------------------------------------------
                    // create a new effector
                    //----------------------------------------------------------
                    effector::ptr eff( new effector(name) );
                    if( ! effectors.insert(eff) )
                        throw exception("in [%s]: multiple effector '%s'", tabname, id);
                    std::cerr << "***\t" << eff->name << std::endl;
                    //----------------------------------------------------------
                    lua_pop(L,1);          //-- pop effector name
                    //----------------------------------------------------------
                    
                    //----------------------------------------------------------
                    lua_rawgeti(L, -1, 2); //-- get the input table
                    //----------------------------------------------------------
                    if( !lua_istable(L, -1) )
                        throw exception("effector '%s': input is not a table",id);
                    
                    __load_strings(id, "input", eff->input, L);
                    std::cerr << "***\t\tinput=" << eff->input << std::endl;
                    //----------------------------------------------------------
                    lua_pop(L, 1);         //-- pop the input table
                    //----------------------------------------------------------
                    
                    //----------------------------------------------------------
                    lua_rawgeti(L, -1, 3); //-- get the input table
                    //----------------------------------------------------------
                    if( !lua_istable(L, -1) )
                        throw exception("effector '%s': output is not a table",id);
                    
                    __load_strings(id, "output", eff->output, L);
                    std::cerr << "***\t\toutput=" << eff->output << std::endl;
                    //----------------------------------------------------------
                    lua_pop(L, 1);         //-- pop the input table
                    //----------------------------------------------------------

                    
                }
                
                
                //--------------------------------------------------------------
                lua_pop(L,1);          //-- pop effector table
                //--------------------------------------------------------------
                
            }
            
        }
        
        void _lua::effector:: call( lua_State *L, solution &dSdt, double t, const solution &S ) const
        {
            const char *fn = name.c_str();
            assert(L);
            lua_settop(L,0);
            lua_getglobal(L,fn);
            if( ! lua_isfunction(L, -1) )
                throw exception("no effector function '%s'", fn);
            
            lua_pushnumber(L, t);
            const size_t nIn = input.size();
            for( size_t i=1; i <= nIn; ++i )
            {
                lua_pushnumber(L, S[ input[i] ]);
            }
            if( lua_pcall(L, 1+nIn, output.size(), 0) )
                throw exception("%s: %s", fn, lua_tostring(L, -1) );
            
            
        }

        
    }
    
}
