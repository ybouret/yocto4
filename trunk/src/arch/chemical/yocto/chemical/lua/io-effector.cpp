#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        class _lua::effector : public chemical::effector
        {
        public:
            mutable lua_State *L;
            explicit effector(const string &id,
                              lua_State    *vm) :
            chemical::effector(id),
            L(vm)
            {
            }
            
            virtual ~effector() throw()
            {
                
            }
            
            virtual void call( YOCTO_CHEMICAL_EFFECTOR_ARGS ) const
            {
                assert(L);
                lua_settop(L,0);
                
                //--------------------------------------------------------------
                //-- push the arguments
                //--------------------------------------------------------------
                lua_getglobal(L,name.c_str());
                
                //--------------------------------------------------------------
                //-- push the arguments
                //--------------------------------------------------------------
                
                // push time
                lua_pushnumber(L,t);
                
                // push zeta
                lua_pushnumber(L,zeta);
                
                // push S
                _lua::push(L,S,lib);
                
                // push S_out
                _lua::push(L,S_out,lib);
                
                //--------------------------------------------------------------
                //-- call the function: 4 args, 1 result
                //--------------------------------------------------------------
                if(lua_pcall(L, 4, 1, 0) != 0)
                    throw exception("effector '%s' error: %s",name.c_str(),lua_tostring(L, -1));
                
                //--------------------------------------------------------------
                //-- get the result
                //--------------------------------------------------------------
                _lua::load(L, dSdt, lib);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
            
        };
        
        void _lua:: load1( lua_State *L, effectors &edb, const string &name, const collection &lib)
        {
            static const char fn[] = "load_effector: ";
            assert(L);
            
            // get the name
            const char *id = name.c_str();
            lua_getglobal(L,id);
            if( !lua_isfunction(L, -1) )
                throw exception("%s%s is not a function",fn,id);
            
            // create an effector
            effector::pointer eff( new _lua::effector(name,L ) );
            if( !edb.insert(eff) )
                throw exception("%smultiple effector '%s'",fn,id);
            
            lua_pop(L,1);
        }
        
        void _lua::load1( lua_State *L, effectors &edb, const char   *name, const collection &lib)
        {
            const string Name(name);
            load1(L, edb, Name, lib);
        }
        
        void _lua:: load(lua_State *L, effectors &edb, const string &list_name, const collection &lib)
        {
            static const char fn[] = "load_effectors: ";
            assert(L);
            const char *id = list_name.c_str();
            lua_getglobal(L,id);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s%s is not a table",fn,id);
            }
            const size_t n = lua_rawlen(L,-1);
            for(size_t i=1;i<=n;++i)
            {
                // get the effector name
                lua_rawgeti(L, -1, i);
                std::cerr << "New Effector: {" << lua_tostring(L, -1) << "}" << std::endl;
                load1(L, edb, lua_tostring(L, -1), lib);
                lua_pop(L, 1);
            }
        }
        
        void _lua:: load(lua_State *L, effectors &edb, const char *list_name, const collection &lib)
        {
            const string ListName(list_name);
            load(L,edb,ListName,lib);
        }
    }
    
}
