#ifndef YOCTO_LINGUA_SYNTAX_ANALYZER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_ANALYZER_INCLUDED 1

#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/hashing/mph.hpp"

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

                //! declare something to do with terminal
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_term( OBJECT_POINTER host, METHOD_POINTER method)
                {
                    onTerm.reset( new term_proc(host,method) );
                }

                //! declare something to do with internal rule
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_rule( OBJECT_POINTER host, METHOD_POINTER method)
                {
                    onRule.reset( new rule_proc(host,method) );
                }

                int hash_term(const string &label) const;
                int hash_rule(const string &label) const;



            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(analyzer);
                auto_ptr<term_proc> onTerm;
                auto_ptr<rule_proc> onRule;
                hashing::mperf      hterm;
                hashing::mperf      hrule;
                int                 depth;
                size_t              max_label_length;
                ios::ostream       *output;
                void __walk( const xnode *node, size_t &ns);
                void emit(const string &label);
            };
        }
    }
}

#endif

