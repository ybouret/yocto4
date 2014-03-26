#ifndef YOCTO_LANG_SYNTAX_JOKER_INCLUDED
#define YOCTO_LANG_SYNTAX_JOKER_INCLUDED 1

#include "yocto/lang/syntax/rule.hpp"

namespace yocto 
{
    namespace lang 
    {
        namespace syntax
        {
            
            //! reference to a previous rule
            class joker : public rule 
            {
            public:
                virtual ~joker() throw();
                
            protected:
                explicit joker( const string &id, rule &r );
                rule &ref;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
            };
            
            //! 0 or 1
            class optional : public joker
            {
            public:
                explicit optional( const string &id, rule &r );
                virtual ~optional() throw();
                
                virtual bool  match( Y_SYNTAX_MATCH_ARGS );  
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
            };
            
            
            //! >= at_least
            class repeating : public joker
            {
            public:
                explicit repeating( const string &id, rule &r, size_t at_least );
                virtual ~repeating() throw();
                virtual bool  match( Y_SYNTAX_MATCH_ARGS );  
                
            private:
                const size_t min_count;
                YOCTO_DISABLE_COPY_AND_ASSIGN(repeating);
            };
            
            //! joker *
            class zero_or_more : public repeating
            {
            public:
                explicit zero_or_more( const string &id, rule &r );
                virtual ~zero_or_more() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(zero_or_more);
            };
            
            
            //! joker +
            class one_or_more : public repeating
            {
            public:
                explicit one_or_more( const string &id, rule &r );
                virtual ~one_or_more() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(one_or_more);
            };

        }
        
    }
}

#endif
