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
        class grammar
        {
        public:
            virtual ~grammar() throw();
            
            explicit grammar(const string &id);
            
            const string name;
            
            //! declare a new terminal
            syntax::terminal & term( const string &label);
            
            //! declare a new terminal
            syntax::terminal & term( const char   *label);
            
            //! declare an optional rule
            syntax::optional & opt( syntax::rule &r );
            
            //! at_least
            syntax::at_least & at_least(const string &label,syntax::rule &, size_t count);
            syntax::at_least & at_least(const char   *label,syntax::rule &, size_t count);

            syntax::at_least & zero_or_more(const char *label,syntax::rule & );
            syntax::at_least & one_or_more( const char *label, syntax::rule & );
            
			//! aggregate
			syntax::aggregate & agg(const string &label);


            //! get a rule
            syntax::rule & operator[](const string &label);
            
            //! get a rule
            syntax::rule & operator[](const char   *label);
            
            
            //! set root
            void set_root( const syntax::rule &r );
            
            syntax::xtree accept( lexer &Lexer, source &Source, ios::istream &Input );
            
            //! show rules on std::cerr
            void show_rules() const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
            
            
            syntax::rule *find_rule(const string &label ) const throw();
            void          ensure_no(const string &label) const;
            
            syntax::r_list rules;
            int            optIndex; //!< to generate optional names
        };
        
    }
}

#endif
