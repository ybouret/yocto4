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
            virtual ~grammar() throw();
            
            explicit grammar(const string &id);
            explicit grammar(const char   *id);
            
            const string name;
            
            //! declare a new terminal, whose label MUST match a lexical rule
            syntax::terminal & term(const string &label, const syntax::xnode_ppty ppty = syntax::is_regular);
            
            //! declare a new terminal, wrapper
            syntax::terminal & term(const char   *label, const syntax::xnode_ppty ppty = syntax::is_regular);
            
            //! declare an optional rule
            syntax::optional & opt( syntax::rule &r );
            
            //! at_least
            syntax::at_least & at_least(const string &label,syntax::rule &, size_t count);
            syntax::at_least & at_least(const char   *label,syntax::rule &, size_t count);

            syntax::at_least & zero_or_more(const char *label,syntax::rule & );
            syntax::at_least & one_or_more( const char *label,syntax::rule & );
            
			//! aggregate of rule
			syntax::aggregate & agg(const string &label, const syntax::xnode_ppty ppty = syntax::is_regular);
            
            //! wrapper
            syntax::aggregate & agg(const char *label, const syntax::xnode_ppty ppty = syntax::is_regular);
            
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
            
            
            syntax::rule *find_rule(const string &label ) const throw();
            void          ensure_no(const string &label) const;
            
            syntax::r_list rules;
            int            optIndex; //!< to generate optional  names
            int            altIndex; //!< to generate alternate names
        };
        
    }
}

#endif
