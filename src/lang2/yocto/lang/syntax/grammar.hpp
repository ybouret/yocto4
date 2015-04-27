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
                rule &term(const string &label);
                //! wrapper
                rule &term(const char   *label);

                //! aggregate
                aggregate &agg( const string &label);
                //! wrapper
                aggregate &agg( const char   *label);

                //! aggregate
                alternate &alt();
                
                //! set root from a previously declared rule
                void set_root( rule &r ) throw();

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

