#ifndef YOCTO_LANG_GRAMMAR_INCLUDED
#define YOCTO_LANG_GRAMMAR_INCLUDED 1

#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/joker.hpp"
#include "yocto/lang/syntax/logic.hpp"

namespace yocto
{
    namespace lang
    {
    
        //! make a grammar
        /**
         the root rule is the first rule
         unless changed by set_root
         */
        class grammar : public virtual object
        {
        public:
            static const syntax::property default_ppty = syntax::is_standard;
            virtual ~grammar() throw();
            
            explicit grammar(const string &id);
            explicit grammar(const char   *id);
            
            const string name;
            
            //! declare a new terminal, whose label MUST match a lexical rule
            syntax::terminal & term(const string &label, const syntax::property ppty = default_ppty);
            
            //! declare a new terminal, wrapper
            syntax::terminal & term(const char   *label, const syntax::property ppty = default_ppty);
            
            //! declare an optional rule
            syntax::optional & opt( syntax::rule &r );
            
            //! at_least
            syntax::at_least & at_least(const string &label,syntax::rule &, size_t count);
            
            //! at_least wrapper
            syntax::at_least & at_least(const char   *label,syntax::rule &, size_t count);

            //! at least >=0
            syntax::at_least & zero_or_more(syntax::rule &,const char *label=NULL);
            
            //! at least >=0
            syntax::at_least & one_or_more(syntax::rule &,const char *label=NULL);
            
            
			//! aggregate of rule
			syntax::aggregate & agg(const string &label, const syntax::property ppty = default_ppty);
            
            //! wrapper
            syntax::aggregate & agg(const char   *label, const syntax::property ppty = default_ppty);
            
            //! alternate rule
            syntax::alternate & alt();
            
            
            //! get a rule
            syntax::rule & operator[](const string &label);
            
            //! get a rule
            syntax::rule & operator[](const char   *label);
            
            
            //! set root
            void set_root( const syntax::rule &r );
            
            //! main code
            syntax::xtree accept( lexer &Lexer, source &Source, ios::istream &Input );
            
            //! show rules on std::cerr
            void show_rules() const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
            
            
            syntax::rule * find_rule(const string &label ) const throw();
            syntax::r_list rules;
            
        protected:
            int            counter; //!< internal counter for auto labeling
            void           ensure_no(const string &label) const;
        };
        
    }
}

#endif
