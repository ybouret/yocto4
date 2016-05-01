#ifndef YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED
#define YOCTO_LINGUA_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lingua/syntax/xnode.hpp"
#include "yocto/associative/glossary.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            typedef functor<void,null_type>            call_type; //!< to process rule
            typedef functor<void,TL1(const string &)>  proc_type; //!< to process terminal
            
            class tree_walker : public object
            {
            public:
                typedef xnode               XNODE;
                typedef call_type           CALL_TYPE;
                typedef proc_type           PROC_TYPE;
                typedef glossary<call_type> callDB;
                typedef glossary<proc_type> procDB;

                virtual ~tree_walker() throw();
                explicit tree_walker();

                void walk(const XNODE *node);

#if 0
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_call(const string &rule_label, OBJECT_POINTER host, METHOD_POINTER method)
                {
                    const call_type cb(host,method);
                    calls.insert(rule_label,cb);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER >
                inline void on_term(const string &term_label, OBJECT_POINTER host, METHOD_POINTER method)
                {
                    const proc_type cb(host,method);
                    procs.insert(term_label,cb);
                }
#endif
                
                
            protected:
                int        depth;
                callDB     calls;
                procDB     procs;
                
                void __walk( const XNODE *node );


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree_walker);
            };
        }
    }
}

#endif
