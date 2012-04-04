#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/plugin.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/core/pool.hpp"

namespace yocto 
{
    namespace regex
    {
        
        class lexer 
        {
        public:
            explicit lexer( const string &init_id);
            explicit lexer( const char   *init_id);
            virtual ~lexer() throw();
            
            sublexer & main() throw();
            sublexer & declare( const string & );
            sublexer & declare( const char   * );
            
            virtual void reset() throw();
            
            sublexer & operator[]( const string & );
            sublexer & operator[]( const char   * );
            
            void jump( const string &sub_name );
            void call( const string &sub_name );
            void back();
            
            void load( lexical::plugin *plg );
            template <typename PLUGIN,typename OBJECT>
            PLUGIN *load( OBJECT *host,void (OBJECT::*method)(void*) )
            {
                const lexical::callback cb(host,method);
                PLUGIN *plg = new PLUGIN( cb );
                load( plg );
                return plg;
            }
            const lexical::plugin &get_plugin( const string &plugin_name ) const;
            
            //! run until end of source
            void run( source &src );
            
            const string &active_name() const throw();
            const char *  active_cstr() const throw();
            
            
        private:
            typedef set<string,sublex>          sublex_set;
            typedef set<string,lexical::module> module_set;
            typedef sublexer                   *lxptr;
            typedef list<lxptr>                 lxptr_stack;
            sublex_set  lexdb;
            sublexer   *active;
            lxptr_stack call_stack;
            sublexer   *init;
            module_set  plugins;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
        
        
    }
    
}

#endif
