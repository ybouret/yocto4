#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/sub-lexer.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/hashing/elf.hpp"

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
            
            void jump( const string &name );
            void call( const string &name );
            void back();
            
            //! run until end of source
            void run( source &src );
            
            
        private:
            typedef set<string,sublex,key_hasher<string,hashing::elf> > sublex_set;
            typedef sublexer         *lxptr;
            typedef list<lxptr>        lxptr_stack;
            
            sublex_set  lexdb;
            sublexer   *active;
            lxptr_stack call_stack;
            sublexer   *init;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
        };
        
        
        
    }
    
}

#endif
