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
            explicit sub_lexer( const string &id );
            explicit sub_lexer( const char   *id );
            virtual ~sub_lexer() throw();
            
            const string   name;
            const string & key() const throw();
            
            void make( pattern *p, const lexical::action &a );
            void make( const string &expr, const lexical::action &a, pattern_dict *dict = NULL );
            void make( const char   *expr, const lexical::action &a, pattern_dict *dict = NULL );
            
            //! apply best rule
            bool process( source &src );
            
            //! recover after a crash...
            void reset() throw();
            
        private:
            lexical::rules rules_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(sub_lexer);
        };
        
        
    }
}
#endif
