#ifndef YOCTO_LANG_GRAMMAR_INCLUDED
#define YOCTO_LANG_GRAMMAR_INCLUDED 1

#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/joker.hpp"
#include "yocto/ordered/catalog.hpp"

namespace yocto 
{
    namespace lang
    {
        //! simple grammar
        /**
         the first declared rule will be the default rule,
         which can be change by set_root
         */
        class grammar 
        {
        public:
            const string name;
            
            //! create an empty grammar
            explicit grammar( const string &id );
            explicit grammar( const char   *id );
            
            //! default ctor
            virtual ~grammar() throw();
            
            //! new terminal
            syntax::terminal &term( const string &id, syntax::node_property = syntax::is_regular);
            syntax::terminal &term( const char   *id, syntax::node_property = syntax::is_regular); 
            
            //! new aggregate
            syntax::aggregate &agg( const string &id, syntax::node_property = syntax::is_regular);
            syntax::aggregate &agg( const char   *id, syntax::node_property = syntax::is_regular);

            //! new alternate
            syntax::alternate &alt( const string &id );
            syntax::alternate &alt( const char   *id );

            //! new optional
            syntax::optional &opt( const string &id, const string &src );
            syntax::optional &opt( const string &id, syntax::rule &ref );
            syntax::optional &opt( const char   *id, const char   *src );
            syntax::optional &opt( const char   *id, syntax::rule &ref );
            
            
            //! new repeating
            syntax::repeating &rep( const string &id, const string &src, size_t at_least);
            syntax::repeating &rep( const string &id, syntax::rule &ref, size_t at_least);
            syntax::repeating &rep( const char   *id, const char   *src, size_t at_least);
            syntax::repeating &rep( const char   *id, syntax::rule &ref, size_t at_least);

            
            //! look up for rule 'id'
            syntax::rule & operator[]( const string &id );
            
            //! look up for rule 'id'
            syntax::rule & operator[]( const char   *id );
            
            //! return a valid parse node
            syntax::parse_node *accept( lexer &Lexer, regex::source &Source );
            
            //! change root rule
            void set_root( const string &id );
            void set_root( const char   *id );
            void set_root( syntax::rule &r  );
            
        private:
            void add( syntax::rule *r );
            
            class item 
            {
            public:
                item( syntax::rule *r  ) throw();
                item( const item   &it ) throw();
                ~item() throw();
                const string &key() const throw();
                typedef set<string,item> db;
                
                syntax::rule *rule;
            private:
                YOCTO_DISABLE_ASSIGN(item);
            };
            
            syntax::rules   rules;
            item::db        items;
            const string    __eof;
            YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
        };
        
        
    }
    
}

#endif
