#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace chemical
    {
        
#if 0
        class _lua:: effector : public chemical::effector
        {
        public:
            lua_State *L;
            vector<string> input;
            vector<string> output;
            
            explicit effector( const string &id, lua_State *vm) :
            chemical::effector(id),
            L(vm),
            input(),
            output()
            {
            }
            
            virtual ~effector() throw()
            {
            }
            
            virtual void call( solution &dSdt, double t, double zeta, const solution &S ) const
            {
                //------------------------------------------------------------------
                // get the function
                //------------------------------------------------------------------
                const char *fn = name.c_str();
                assert(L);
                lua_settop(L,0);
                lua_getglobal(L,fn);
                if( ! lua_isfunction(L, -1) )
                    throw exception("no effector function '%s'", fn);
                
                //------------------------------------------------------------------
                // push the arguments
                //------------------------------------------------------------------
                lua_pushnumber(L, t);
                lua_pushnumber(L, zeta);
                const size_t nIn = input.size();
                for( size_t i=1; i <= nIn; ++i )
                {
                    lua_pushnumber(L, S[ input[i] ]);
                }
                
                //------------------------------------------------------------------
                // call the function
                //------------------------------------------------------------------
                const size_t nOut = output.size();
                if( lua_pcall(L, 2+nIn, nOut, 0) )
                    throw exception("%s: %s", fn, lua_tostring(L, -1) );
                
                //------------------------------------------------------------------
                // pop the results
                //------------------------------------------------------------------
                for( size_t i=nOut;i>0;--i)
                {
                    if( !lua_isnumber(L, -1) )
                        throw exception("%s: invalid result #%u", fn, unsigned(i));
                    dSdt[ output[i] ] +=  lua_tonumber(L, -1);
                    lua_pop(L,1);
                }
                
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
        };
        
        static inline
        void __parse_strings_table( lua_State *L, const char *effname, vector<string> &seq, const char *which)
        {
            assert(L);
            assert(effname);
            assert(which);
            if( !lua_istable(L, -1))
                throw exception("%s %s is not a LUA_TABLE", effname, which);
            const size_t n = lua_rawlen(L, -1);
            for(size_t i=1; i <=n; ++i )
            {
                lua_rawgeti(L, -1, i);
                if( !lua_isstring(L, -1) )
                    throw exception("%s %s[%u] is not a string", effname, which, unsigned(i));
                const string s = lua_tostring(L, -1);
                seq.push_back(s);
                lua_pop(L,1);
            }
            
        }
        
        static inline
        void __parse_effector( lua_State *L, effectors &eff, const char *table_name, const unsigned table_indx )
        {
            if( !lua_istable(L, -1))
                throw exception("%s[%u] is not a LUA_TABLE", table_name, table_indx);
            
            if( 3 != lua_rawlen(L, -1))
                throw exception("%s[%u] invalid format", table_name, table_indx);
            
            //------------------------------------------------------------------
            // get the function name
            //------------------------------------------------------------------
            lua_rawgeti(L, -1, 1);
            if(!lua_isstring(L, -1))
                throw exception("%s[%u] field #1 is not a string", table_name, table_indx);
            const string name = lua_tostring(L, -1);
            lua_pop(L,1);
            
            const char *effname = name.c_str();
            lua_getglobal( L, effname );
            if( !lua_isfunction(L, -1 ))
                throw exception("no effector function '%s'", effname);
            lua_pop(L,1);
            
            _lua::effector *p = new _lua::effector(name,L);
            const effector::ptr q(p);
            
            std::cerr << "Effector '" << name << "': ";
            //------------------------------------------------------------------
            // get the input name
            //------------------------------------------------------------------
            lua_rawgeti(L, -1, 2);
            __parse_strings_table(L, effname, p->input, "input" );
            lua_pop(L,1);
            std::cerr << p->input << " => ";
            //------------------------------------------------------------------
            // get the output names
            //------------------------------------------------------------------
            lua_rawgeti(L, -1, 3);
            __parse_strings_table(L, effname, p->output, "output" );
            lua_pop(L,1);
            std::cerr << p->output << std::endl;
            if( !eff.insert(q) )
                throw exception("multiple effector '%s'", effname);
        }
#endif
        
        
        class _lua:: effector : public chemical::effector
        {
        public:
            lua_State *L;
            
            
            explicit effector( const string &id, lua_State *vm) :
            chemical::effector(id),
            L(vm)
            {
            }
            
            virtual ~effector() throw()
            {
            }
            
            virtual void call( solution &dSdt, double t, double zeta, const solution &S, const solution &S_out ) const
            {
                //--------------------------------------------------------------
                // get the function
                //--------------------------------------------------------------
                const char *fn = name.c_str();
                assert(L);
                lua_settop(L,0);
                lua_getglobal(L,fn);
                if( ! lua_isfunction(L, -1) )
                    throw exception("no effector function '%s'", fn);
                
                //--------------------------------------------------------------
                // push the arguments
                //--------------------------------------------------------------
                lua_pushnumber(L, t);
                lua_pushnumber(L, zeta);
                _lua::push_solution(L,S);
                _lua::push_solution(L,S_out);
                
                //--------------------------------------------------------------
                // call the function
                //--------------------------------------------------------------
                if( lua_pcall(L,4,1,0) )
                    throw exception("%s.call: %s", fn, lua_tostring(L,-1) );
                
                if( !lua_istable(L,-1) )
                    throw exception("effector '%s' doesn't return a table", name.c_str());
                
                //--------------------------------------------------------------
                // parse the result
                //--------------------------------------------------------------
                const int ans = lua_gettop(L); // the result index
                lua_pushnil(L);  /* first key */
                while( lua_next(L,ans) != 0) {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    //printf("%s - %s\n",lua_typename(L, lua_type(L, -2)),lua_typename(L, lua_type(L, -1)));
                    
                    if( !lua_isstring(L,-2) )
                        throw exception("effector '%s': return key is not 'string' but '%s'", fn, lua_typename(L, lua_type(L, -2)));
                    
                    if( !lua_isnumber(L,-1) )
                        throw exception("effector '%s': return value is not 'number' but '%s'", fn, lua_typename(L, lua_type(L, -1)));
                    
                    const string which = lua_tostring(L,-2);
                    const double rate  = lua_tonumber(L,-1);
                    
                    component *cc = dSdt.lookup(which);
                    if(!cc)
                    {
                        throw exception("No [%s] to modify for effector '%s'", which.c_str(), fn);
                    }
                    
                    cc->concentration = rate;
                    
                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
        };
        
        
        
        void _lua:: load( lua_State *L, effectors &eff, const string &name)
        {
            assert(L);
            lua_settop(L,0);
            const char *table_name = name.c_str();
            lua_getglobal(L,table_name);
            
            if( !lua_istable(L, -1) )
                throw exception("%s is not a LUA_TABLE", table_name);
            
            const unsigned n = lua_rawlen(L, -1);
            std::cerr << "Parsing " << n << " effectors" << std::endl;
            for(unsigned i=1; i<=n; ++i)
            {
                lua_rawgeti(L,-1,i);
                
                if( !lua_isstring(L, -1) )
                    throw exception("effector #%u: not a string", i);
                
                const string id = lua_tostring(L,-1);
                std::cerr << "\tcreating effector " << id << std::endl;
                const effector::ptr q(new _lua::effector(id,L) );
                if( !eff.insert(q) )
                    throw exception("multiple effector '%s'", id.c_str());
                lua_pop(L,1);
            }
        }
        
        void _lua::load( lua_State *L, effectors &eff, const char *name)
        {
            const string ID(name);
            _lua::load(L,eff,ID);
        }
        
        
    }
}

