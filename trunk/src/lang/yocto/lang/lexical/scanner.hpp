#ifndef YOCTO_LANG_SCANNER_INCLUDED
#define YOCTO_LANG_SCANNER_INCLUDED 1

#include "yocto/lang/lexeme.hpp"
#include "yocto/lang/lexical/rule.hpp"
#include "yocto/lang/pattern/compiler.hpp"
#include "yocto/counted.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/ptr/auto.hpp"


namespace yocto
{
    namespace lang
    {
        class lexer;
        
        namespace lexical
        {
            
            typedef functor<void,TL1(const token &)> callback;
            
            class scanner : public object, public counted
            {
            public:
                typedef intr_ptr<string,scanner> pointer;
                
                //! default ctor
                explicit scanner( const string &id, int &line_ref, const p_dict &dict_ref );
                
                //! default ctor
                explicit scanner( const char   *id, int &line_ref, const p_dict &dict_ref );
                virtual ~scanner() throw();
                
                const string name; //!< its name for lexer
                int         &line; //!< a line number reference
                
                const string & key() const throw(); //!< return the name
                
                void reset() throw(); //!< reset all motif from content
                
                //! append a rule while checking name, delete upon error
                void append( rule *r );
                
                //! low-level wrapper
                void append(const string    &label,
                            pattern         *motif,
                            const action    &which,
                            const rule::kind flag);
                
                //! low-level wrapper
                void append(const char      *label,
                            pattern         *motif,
                            const action    &which,
                            const rule::kind flag);
                
                
                
                
                //! lexeme scanning protocol
                /**
                 find the first longest matching pattern and make
                 a lexeme out of it.
                 - if returns != NULL, a valid lexeme
                 - if returns NULL:
                 -- if is_control==false => EOF
                 -- if is_control==true  => control rule was met
                 */
                lexeme *get( source &src, ios::istream &fp, bool &is_control );
                
                //! link to a lexer
                void link_to( lexer & );
                
                
                
                //--------------------------------------------------------------
                //
                // make rules: simple undertaken actions
                //
                //--------------------------------------------------------------
                inline void make(const string &label,
                                 pattern      *motif,
                                 const action &which)
                {
                    append(label, motif, which, rule::is_regular);
                }
                
                inline void  make(const char   *label,
                                  pattern      *motif,
                                  const action &which)
                {
                    append(label, motif, which, rule::is_regular);
                }
                
                
                inline void make(const string &label,
                                 const string &regex,
                                 const action &which)
                {
                    append(label, compile(regex,dict), which, rule::is_regular);
                }
                
                inline void make(const char  *label,
                                 const char  *regex,
                                 const action &which)
                {
                    append( label, compile(regex,dict), which, rule::is_regular);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void make(const string  &label,
                          pattern       *motif,
                          OBJECT_POINTER Object,
                          METHOD_POINTER Method)
                {
                    auto_ptr<pattern> p(motif);
                    const action which(Object,Method);
                    make(label,p.yield(),which);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void make(const char    *label,
                          pattern       *motif,
                          OBJECT_POINTER Object,
                          METHOD_POINTER Method)
                {
                    auto_ptr<pattern> p(motif);
                    const action which(Object,Method);
                    make(label,p.yield(),which);
                }
                
                
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void make(const string &label,
                          const string &regex,
                          OBJECT_POINTER Object,
                          METHOD_POINTER Method)
                {
                    const action which(Object,Method);
                    make(label,regex,which);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void make(const char    *label,
                          const char    *regex,
                          OBJECT_POINTER Object,
                          METHOD_POINTER Method)
                {
                    const action which(Object,Method);
                    make(label,regex,which);
                }
                
                
                bool    emit(const token &);
                bool    drop(const token &);
                bool    newline(const token &);        //!< increase #line, discard
                bool    newline_emit(const token &);   //!< increase #line, forward
                
                
                //--------------------------------------------------------------
                //
                // jump: jumping to a lexer's subscanner
                //
                //--------------------------------------------------------------
                
                //! change lexer's scanner with a jump
                void jump(const string   &target,
                          pattern        *motif,
                          const callback &onJump);
                
                //! jump wrapper
                inline void jump(const char     *target,
                                 pattern        *motif,
                                 const callback &onJump)
                {
                    auto_ptr<pattern> q(motif);
                    const string Target(target);
                    jump(Target,q.yield(),onJump);
                }
                
                //! jump wrapper
                inline void jump(const string   &target,
                                 const string   &regex,
                                 const callback &onJump)
                {
                    jump(target,compile(regex,dict),onJump);
                }
                
                //! jump wrapper
                inline void jump(const char *target,
                                 const char *regex,
                                 const callback &onJump)
                {
                    const string Target(target);
                    jump( Target, compile(regex,dict), onJump);
                }
                
                
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void jump(const string   &target,
                                 pattern        *motif,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    auto_ptr<pattern> q(motif);
                    const callback onJump(Object,Method);
                    jump(target,q.yield(),onJump);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void jump(const char     *target,
                                 pattern        *motif,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    auto_ptr<pattern> q(motif);
                    const callback onJump(Object,Method);
                    jump(target,q.yield(),onJump);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void jump(const string   &target,
                                 const string   &regex,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    const callback onJump(Object,Method);
                    jump(target,regex,onJump);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void jump(const char     *target,
                                 const char     *regex,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    const callback onJump(Object,Method);
                    jump(target,regex,onJump);
                }
                
                
                //--------------------------------------------------------------
                //
                // call: calling a lexer's subscanner
                //
                //--------------------------------------------------------------
                
                //! change lexer's scanner with a call
                void call(const string   &target,
                          pattern        *motif,
                          const callback &onCall);
                
                //! call wrapper
                inline void call(const char     *target,
                                 pattern        *motif,
                                 const callback &onCall)
                {
                    auto_ptr<pattern> q(motif);
                    const string Target(target);
                    call(Target,q.yield(),onCall);
                }
                
                inline void call(const string   &target,
                                 const string   &regex,
                                 const callback &onCall)
                {
                    call(target,compile(regex,dict),onCall);
                }
                
                inline void call(const char     *target,
                                 const char     *regex,
                                 const callback &onCall)
                {
                    call(target,compile(regex,dict),onCall);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void call(const string   &target,
                                 pattern        *motif,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    auto_ptr<pattern> q(motif);
                    const callback onCall(Object,Method);
                    call(target,q.yield(),onCall);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void call(const char     *target,
                                 pattern        *motif,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    auto_ptr<pattern> q(motif);
                    const callback onCall(Object,Method);
                    call(target,q.yield(),onCall);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void call(const string   &target,
                                 const string   &regex,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    const callback onCall(Object,Method);
                    call(target,regex,onCall);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void call(const char     *target,
                                 const char     *regex,
                                 OBJECT_POINTER  Object,
                                 METHOD_POINTER  Method)
                {
                    const callback onCall(Object,Method);
                    call(target,regex,onCall);
                }
                
                
                
                //--------------------------------------------------------------
                //
                // back: returning to a lexer's calling subscanner
                //
                //--------------------------------------------------------------
                void back(pattern        *motif,
                          const callback &onBack);
                
                inline void back(const string   &regex,
                                 const callback &onBack)
                {
                    back( compile(regex,dict), onBack);
                }
                
                inline void back(const char     *regex,
                                 const callback &onBack)
                {
                    back( compile(regex,dict), onBack);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void back(pattern       *motif,
                                 OBJECT_POINTER Object,
                                 METHOD_POINTER Method)
                {
                    auto_ptr<pattern> q(motif);
                    const callback    onBack(Object,Method);
                    back( q.yield(), onBack );
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void back(const string  &regex,
                                 OBJECT_POINTER Object,
                                 METHOD_POINTER Method)
                {
                    const callback onBack(Object,Method);
                    back( regex, onBack);
                }
                
                template <
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void back(const char    *regex,
                                 OBJECT_POINTER Object,
                                 METHOD_POINTER Method)
                {
                    const callback onBack(Object,Method);
                    back( regex, onBack);
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
                r_list        rules;
                lexer        *mylex;
                const p_dict *dict;
                int           iBack;
                void append_context_to( string &s ) const;
            };
            
        }
    }
}

#endif
