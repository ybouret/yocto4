#ifndef YOCTO_LINGUA_SYNTAX_ANALYZER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_ANALYZER_INCLUDED 1

#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            class analyzer
            {
            public:
                typedef functor<void,TL2(const string&,const string&)> term_proc;
                typedef functor<void,TL2(const string&,const size_t )> rule_proc;

                virtual ~analyzer() throw();
                explicit analyzer(const grammar &g);

                void walk(const xnode *tree, ios::ostream *fp);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(analyzer);
                auto_ptr<term_proc> onTerm;
                auto_ptr<rule_proc> onRule;
                int                depth;
                size_t              max_label_length;
                ios::ostream       *output;
                void __walk( const xnode *node, size_t &ns);
                void emit(const string &label);

            };
        }
    }
}

#endif

