#ifndef YOCTO_LINGUA_GRAMMAR_INCLUDED
#define YOCTO_LINGUA_GRAMMAR_INCLUDED 1

#include "yocto/lingua/syntax/terminal.hpp"
#include "yocto/lingua/syntax/aggregate.hpp"
#include "yocto/lingua/syntax/alternative.hpp"

#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace lingua
    {
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
            
            //! new terminal
            syntax::terminal &term( const char   *id, syntax::node_property = syntax::is_regular);

            //! new aggregate
            syntax::aggregate &agg( const string &id, syntax::node_property = syntax::is_regular);
            
            //! new aggregate
            syntax::aggregate &agg( const char   *id, syntax::node_property = syntax::is_regular);
            
            //! new alternative
            syntax::alternative &alt( const string &id );
            
            //! new alternative
            syntax::alternative &alt( const char   *id );
            
           
            void set_root( const string &rule_id );  //!< changing root
            void set_root( const char   *rule_id );  //!< changing root
            void set_root( const syntax::rule &r );  //!< changing root
            
            //! convert root to tree
            bool accept(lexer &Lexer, source &Source, syntax::xnode * &Tree);
            
            syntax::rule & operator[]( const string &id );
            syntax::rule & operator[]( const char   *id );
            
            
        private:
            void add( syntax::rule *r);
            
            class item
            {
            public:
                item( syntax::rule::ptr p ) throw();
                ~item() throw();
                item(const item &) throw();
                const string &key() const throw();
                
                syntax::rule::ptr handle;
                typedef set<string,item> db;
            private:
                YOCTO_DISABLE_ASSIGN(item);
            };
            
            syntax::r_list rules; //!< first is root, can be changed with set_root
            item::db       items; //!< for rule fast naming
            const string   __eof; //!< "EOF"
            YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
        };
    }
}

#endif
