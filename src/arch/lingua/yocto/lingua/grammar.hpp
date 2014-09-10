#ifndef YOCTO_LINGUA_GRAMMAR_INCLUDED
#define YOCTO_LINGUA_GRAMMAR_INCLUDED 1

#include "yocto/lingua/syntax/terminal.hpp"
#include "yocto/lingua/syntax/aggregate.hpp"
#include "yocto/lingua/syntax/alternative.hpp"
#include "yocto/lingua/syntax/joker.hpp"

#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace lingua
    {
        class grammar : public virtual object
        {
        public:
            const string name;
            
            //! create an empty grammar
            explicit grammar( const string &id );
            
            //! create an empty grammar
            explicit grammar( const char   *id );
            
            //! default ctor
            virtual ~grammar() throw();
            
            //! new terminal, MUST match a lexeme's label
            syntax::terminal &term( const string &id, syntax::node_property = syntax::is_regular);
            
            //! wrapper
            syntax::terminal &term( const char   *id, syntax::node_property = syntax::is_regular);

            //! new aggregate
            syntax::aggregate &agg( const string &id, syntax::node_property = syntax::is_regular);
            
            //! new aggregate
            syntax::aggregate &agg( const char   *id, syntax::node_property = syntax::is_regular);
            
            //! new alternative
            syntax::alternative &alt( const string &id );
            
            //! new alternative
            syntax::alternative &alt( const char   *id );
            
            //! automatic naming since it shall not appear.
            syntax::alternative &alt();
            
            //! new optional, automatic naming
            syntax::optional &opt(syntax::rule &r);
          
            //! new repeating
            /**
             with syntax::is_merging_all property
             */
            syntax::repeating &rep( const string &id, syntax::rule &r, size_t at_least);
            
            //! new repeating
            syntax::repeating &rep( const char   *id, syntax::rule &r, size_t at_least);

            void set_root( const string &rule_id );  //!< changing root
            void set_root( const char   *rule_id );  //!< changing root
            void set_root( const syntax::rule &r );  //!< changing root
            
            //! convert root to tree
            /**
             throw upon error
             */
            syntax::xnode *accept(lexer &Lexer, source &Source );
            
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
            
            void check_ownership( const syntax::rule &r ) const;
            
            syntax::r_list rules; //!< first is root, can be changed with set_root
            item::db       items; //!< for rule fast naming
            const string   __eof; //!< "EOF"
            int            opidx; //!< for automatic naming
            YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
        };
    }
}

#endif
