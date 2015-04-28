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
                rule &decl_term(const string &label);

                //! wrapper
                rule &decl_term(const char   *label);

                //! aggregate
                aggregate &agg( const string &label);
                //! wrapper
                aggregate &agg( const char   *label);

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


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
                r_list rules;
                void check_label(const string &label) const;

            };
        }
        
    }
    
}

#endif

