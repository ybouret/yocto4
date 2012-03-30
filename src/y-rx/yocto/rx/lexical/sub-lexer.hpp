#ifndef YOCTO_RX_sublexer_INCLUDED
#define YOCTO_RX_sublexer_INCLUDED 1

#include "yocto/rx/lexical/rule.hpp"
#include "yocto/intrusive-ptr.hpp"

namespace yocto
{
    namespace regex
    {
        
        class pattern_dict;
        class sublexer : public object, public counted
        {
        public:
            explicit sublexer( const string &id, lexer *lx = NULL );
            explicit sublexer( const char   *id, lexer *lx = NULL );
            virtual ~sublexer() throw();
            
            const string   name;
            const string & key() const throw();
            
            //==================================================================
            // make API: append a rule
            //==================================================================
            //! create or update a an action for a given pattern
            void make( pattern *p, const lexical::action &a );
            void make( const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void make( const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            template <typename OBJECT>
            void make( const char *expr, OBJECT *obj, void (OBJECT::*method)( const token &), pattern_dict *dict = NULL )
            {
                const lexical::action a(obj,method);
                this->make(expr,a,dict);
            }
            
            
            //==================================================================
            // join API: append a rule before the last one !
            //==================================================================
            //! create or update a an action for a given pattern
            void join( pattern *p, const lexical::action &a );
            void join( const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void join( const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            template <typename OBJECT>
            void join( const char *expr, OBJECT *obj, void (OBJECT::*method)( const token &), pattern_dict *dict = NULL )
            {
                const lexical::action a(obj,method);
                this->join(expr,a,dict);
            }
            
            
            //==================================================================
            // jump API
            //==================================================================
            void jump(const string &name, pattern *p, const lexical::action &a  );
            void jump(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void jump(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            //==================================================================
            // call API
            //==================================================================
            void call(const string &name, pattern *p, const lexical::action &a  );
            void call(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void call(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            //==================================================================
            // back API
            //==================================================================
            void back( pattern *p, const lexical::action &a );
            void back( const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void back( const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            
            //! apply best rule
            bool process( source &src );
            
            //! recover after a crash...
            void reset() throw();
            
            //! change parent lexer
            void attach( lexer *lx ) throw();
            
            
            //! append a plugin
            void plug( const string &plugin_name );
            void plug( const char   *plugin_name );
            
            void discard( const token & ) throw();
            
        private:
            bool could_append( pattern *p, const lexical::action &a );
            
            lexical::rules rules_;
            lexer         *parent;
            YOCTO_DISABLE_COPY_AND_ASSIGN(sublexer);
        };
        
        typedef intrusive_ptr<string,sublexer> sublex;
        
        
    }
}
#endif
