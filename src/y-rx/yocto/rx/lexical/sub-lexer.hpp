#ifndef YOCTO_RX_SUB_LEXER_INCLUDED
#define YOCTO_RX_SUB_LEXER_INCLUDED 1

#include "yocto/rx/lexical/rule.hpp"
#include "yocto/intrusive-ptr.hpp"

namespace yocto
{
    namespace regex
    {
        
        class pattern_dict;
        class sub_lexer : public object, public counted
        {
        public:
            explicit sub_lexer( const string &id, lexer *lx = NULL );
            explicit sub_lexer( const char   *id, lexer *lx = NULL );
            virtual ~sub_lexer() throw();
            
            const string   name;
            const string & key() const throw();
            
            void make( pattern *p, const lexical::action &a );
            void make( const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void make( const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            void jump(const string &name, pattern *p, const lexical::action &a  );
            void jump(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void jump(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );

            
            void call(const string &name, pattern *p, const lexical::action &a  );
            void call(const string &name, const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void call(const char   *name, const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            void back( pattern *p, const lexical::action &a );
            void back( const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void back( const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            
            //! apply best rule
            bool process( source &src );
            
            //! recover after a crash...
            void reset() throw();
            
            //! change parent lexer
            void attach( lexer *lx ) throw();
            
        private:
            lexical::rules rules_;
            lexer         *parent;
            YOCTO_DISABLE_COPY_AND_ASSIGN(sub_lexer);
        };
        
        typedef intrusive_ptr<string,sub_lexer> sublex;
        
        
    }
}
#endif
