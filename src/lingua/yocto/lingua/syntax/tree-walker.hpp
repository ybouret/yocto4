#ifndef YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/associative/glossary.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            typedef functor<void,null_type>            rule_proc; //!< to process rule
            typedef functor<void,TL1(const string &)>  term_proc; //!< to process terminal
            
            class tree_walker : public object
            {
            public:
                typedef xnode               XNODE;
                typedef glossary<rule_proc> rule_proc_db;
                typedef glossary<term_proc> term_proc_db;

                virtual ~tree_walker() throw();
                explicit tree_walker(const grammar &G);

                void walk(const XNODE *node);

                //! declare something to do with internal rule
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_rule(const string &label, OBJECT_POINTER host, METHOD_POINTER method)
                {
                    check_is_rule(label);
                    const rule_proc cb(host,method);
                    rule_procs.insert(label,cb);
                }

                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_rule(const char *label, OBJECT_POINTER host, METHOD_POINTER method)
                {
                    const string Label(label); on_rule(label,host,method);
                }


                //! declare something to do with terminal
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_term(const string &label, OBJECT_POINTER host, METHOD_POINTER method)
                {
                    check_is_term(label);
                    const term_proc cb(host,method);
                    term_procs.insert(label,cb);
                }

                //! wrapper
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_term(const char *label, OBJECT_POINTER host, METHOD_POINTER method)
                {
                    const string Label(label); on_term(label,host,method);
                }




            private:
                int                  depth;
                rule_proc_db         rule_procs;
                term_proc_db         term_procs;
                map<string,uint32_t> uuids;
                void __walk( const XNODE *node, size_t &ns);
                uint32_t uuid_for(const string &label) const;
                void     check_is_rule(const string &label) const;
                void     check_is_term(const string &label) const;

                YOCTO_DISABLE_COPY_AND_ASSIGN(tree_walker);
            };
        }
    }
}

#endif
