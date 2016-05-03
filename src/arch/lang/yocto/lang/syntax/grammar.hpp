#ifndef YOCTO_LANG_GRAMMAR_INCLUDED
#define YOCTO_LANG_GRAMMAR_INCLUDED 1

#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/aggregate.hpp"

namespace yocto
{
    namespace lang
    {

        namespace syntax
        {
            class grammar : public object
            {
            public:
                explicit grammar(const string &id);
                explicit grammar(const char   *id);
                virtual ~grammar() throw();

                const string name;

                //! accepting terminal
                terminal &decl_term(const string &label, const property ppty=standard);

                //! wrapper
                terminal &decl_term(const char   *label, const property ppty=standard);

                //! aggregate
                aggregate &agg( const string &label, const property ppty=standard);
                //! wrapper
                aggregate &agg( const char   *label, const property ppty=standard);

                //! aggregate
                alternate &alt();

                //! optional
                rule &opt( rule & );

                //! at least
                rule &at_least(rule &r, const size_t nmin);
                rule &zero_or_more( rule &r );
                rule &one_or_more( rule &r );
                
                //! set top level from a previously declared rule
                void top_level( rule &r ) throw();

                //! try to accept first rule
                xnode *accept( lexer &lxr, source &src, ios::istream &fp);

                //! build Graphviz
                void gramviz( const string &filename ) const;

                //! build Graphviz wrapper
                void gramviz( const char   *filename ) const;

                //! find rule, grammar must be UNLOCKED
                bool  has(const string &id) const throw();

                //! find rule, grammar must be UNLOCKED
                bool  has(const char   *id) const;

                //! find rule, grammar must be UNLOCKED
                bool  has(const rule   *r ) const throw();

                //! get the rule, grammar must be UNLOCKED
                rule &get_rule(const string &id);

                //! get the rule, grammar must be UNLOCKED
                rule &get_rule(const char   *id);

                //! slowly fetch rule, NULL if no one found
                const rule *fetch(const string &ID) const throw();

                //! append a rule
                void append( rule *r );

                //! return current top level rule
                rule &top_level();

                size_t count() const throw();

                void cleanup(); //! remove empty aggregate

                //! remove a rule with given label
                void remove( const string &label );

                //! delete internal database and compile aggregates followers
                void finalize();

                void show_followers() const;

                void walker_prolog( ios::ostream &fp, const string &class_name) const;
                void walker_epilog( ios::ostream &fp, const string &class_name) const;

                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
                class  dbase;
                r_list rules;
                dbase *db;    //!< internal database of rules, deleted on lock()
                
                void check_label(const string &label) const;
                void check_locked() const;
                void enroll( rule *r );
                void failure(lexer &lxr);
            };
        }
        
    }
    
}

#endif

