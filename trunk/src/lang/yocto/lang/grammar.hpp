#ifndef YOCTO_LANG_GRAMMAR_INCLUDED
#define YOCTO_LANG_GRAMMAR_INCLUDED 1

#include "yocto/lang/syntax/terminal.hpp"

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
            
            //! get a rule
            syntax::rule & operator[](const string &label);
            
            //! get a rule
            syntax::rule & operator[](const char   *label);
            
            
            //! set root
            void set_root( const string &label );
            
            //! set root
            void set_root( const char   *label );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(grammar);
            
            syntax::r_list rules;
            
            syntax::rule *find_rule(const string &label ) const throw();
        };
        
    }
}

#endif
