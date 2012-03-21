#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/sub-lexer.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"

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
            
            sub_lexer & main() throw();
            sub_lexer & declare( const string & );
            sub_lexer & declare( const char   * );
        
            virtual void reset() throw();
            
            sub_lexer & operator[]( const string & );
            sub_lexer & operator[]( const char   * );
            
            void jump( const string &name );
            void call( const string &name );
            void back();
            
            //! run until end of source
            void run( source &src );
            
            
        private:
            typedef set<string,sublex> sublex_set;
            typedef sub_lexer         *lxptr;
            typedef list<lxptr>        lxptr_stack;
            
            sublex_set  lexdb;
            sub_lexer  *active;
            lxptr_stack call_stack;
            sub_lexer  *init;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
        
        
    }
    
}

#endif
