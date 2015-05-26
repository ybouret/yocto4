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

                //! build GraphViZ..
                void gramviz( const string &filename ) const;

                
                bool  has(const string &id) const throw();
                bool  has(const char   *id) const;
                bool  has(const rule   *r ) const throw();
                
                rule &get_rule(const string &id);
                rule &get_rule(const char   *id);


                void append( rule *r );

                rule &top_level();

                size_t count() const throw();

                void cleanup(); //! remove empty aggregate
                void remove( const string &label );
                
                void lock() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
                class  dbase;
                r_list rules;
                dbase *db;
                
                void check_label(const string &label) const;
                void check_locked() const;
                void enroll( rule *r );
                void failure(lexer &lxr);
            };
        }
        
    }
    
}

#endif

