#ifndef YOCTO_LINGUA_LEXICAL_RULES_INCLUDED
#define YOCTO_LINGUA_LEXICAL_RULES_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            //__________________________________________________________________
            //
            //! what to do with a token
            /**
             if returns true  => will produce a lexeme
             if returns false => lexeme will be discarded
             */
            //__________________________________________________________________
            typedef functor<bool,TL1(const token&)> action;
            
            //__________________________________________________________________
            //
            //! generic rule
            //__________________________________________________________________
            class rule : public object
            {
            public:
              
                const string   label;   //!< terminal name
                pattern       *motif;   //!< accepting pattern
                rule          *prev;    //!< for linked list
                rule          *next;    //!< for linked list
                
                
                //! create a rule
                /**
                 \param id       will be rule label
                 \param p        will be rule motif: taken care of in case of error
                 \param cb       callback when motif is selected
                 \param control  will be its role...
                 the id cannot be EOF !
                 */
                static rule * create( const string &id, pattern *p, const action &cb, bool control);
                static void   kill( rule *r ) throw();
                
                //! return proc( *motif )
                /**
                 when motif is selected, call this->proc( *motif );
                 \warning a control rule can't produce a lexeme !!
                 */
                bool produce();
                
            private:
                action proc;
                explicit rule( const string &id, pattern *p, const action &cb, bool control);
                virtual ~rule() throw();
                YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
            public:
                const bool fctl;    //!<  true if role is jump/call/back
            };
         
            //__________________________________________________________________
            //
            //! list of rules
            //__________________________________________________________________
            class r_list : public core::list_of<rule>
            {
            public:
                explicit r_list() throw();
                virtual ~r_list() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(r_list);
            };
            
        }
    }
}
#endif
