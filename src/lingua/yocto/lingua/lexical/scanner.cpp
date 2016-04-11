#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            scanner:: ~scanner() throw()
            {
            }

            const string & scanner:: key() const throw()
            {
                return name;
            }


#define Y_SCANNER_CTOR() , lex(0), dict(0), rules()

            scanner:: scanner(const string &id, int &line_ref) :
            name(id),
            line(line_ref) Y_SCANNER_CTOR()
            {
            }


            scanner:: scanner(const char *id, int &line_ref) :
            name(id),
            line(line_ref) Y_SCANNER_CTOR()
            {
            }


            void scanner:: check(const string &label)
            {
                if(label.size()<=0) throw exception("scanner: empty rule label");

                for(const rule *r=rules.head;r;r=r->next)
                {
                    if(label==r->label) throw exception("scanner: multiple rule '%s'", label.c_str());
                }

            }

        }
    }
}

#include "yocto/lingua/pattern/posix.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            bool scanner:: forward(const token&) throw() { return true;  }
            bool scanner:: discard(const token&) throw() { return false; }
            bool scanner:: newline(const token&) throw() { ++line; return false; }
            void scanner:: endl_cb(const token&) throw() { ++line; }
            void scanner:: drop_cb(const token&) throw() {  }

            void scanner::make(const string &l,pattern *p,const action &a)
            {
                const pattern::ptr q(p);
                check(l);
                rules.push_back( new rule(l,q,a,false) );
            }

            void scanner:: emit(const string &label, const string &expr)
            {
                make(label,expr,this, &scanner::forward);
            }

            void scanner:: emit(const char *label, const char *expr)
            {
                make(label,expr,this, &scanner::forward);
            }

            void scanner:: drop(const string &label, const string &expr)
            {
                make(label,expr,this, &scanner::discard);
            }

            void scanner:: drop(const char *label, const char *expr)
            {
                make(label,expr,this, &scanner::discard);
            }


            void scanner:: endl(const string &label)
            {
                const action a(this,&scanner::newline);
                make(label, posix::endl(), a);
            }

            void scanner:: endl(const char *label)
            {
                const string l(label);
                const action a(this,&scanner::newline);
                make(l, posix::endl(), a);
            }

        }

    }
}

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            lexeme *scanner:: get( source &src, bool &ctrl)
            {


                while(true)
                {
                    //__________________________________________________________
                    //
                    // check source status
                    //__________________________________________________________
                    if(!src.is_active()) return 0;

                    //__________________________________________________________
                    //
                    //
                    // find a first matching pattern
                    //
                    //__________________________________________________________

                    //__________________________________________________________
                    //
                    // loop
                    //__________________________________________________________
                    rule *best_rule  = rules.head;
                    token best_tokn;
                    for(;best_rule;best_rule=best_rule->next)
                    {
                        if(best_rule->motif->match(best_tokn,src) )
                        {
                            src.unread_a_copy_of(best_tokn);
                            break;
                        }
                        assert(0==best_tokn.size);
                    }

                    //__________________________________________________________
                    //
                    // syntax error
                    //__________________________________________________________
                    if(!best_rule)
                    {
                        assert(src.peek());
                        const uint8_t bad = src.peek()->code;
                        if(bad>=32&&bad<127)
                        {
                            throw exception("%d:<%s> bad char '%c'", line, name.c_str(), bad);
                        }
                        else
                        {
                            throw exception("%d:<%s> bad char 0x%02d", line, name.c_str(), bad);
                        }
                    }


                    //__________________________________________________________
                    //
                    //
                    // check following rules
                    //
                    //__________________________________________________________
                    for(rule *r=best_rule->next;r;r=r->next)
                    {
                        token t;
                        if( !r->motif->match(t,src) )
                        {
                            assert(0==t.size);
                            continue;
                        }

                        //______________________________________________________
                        //
                        // we have a possible better expression
                        //______________________________________________________
                        if(t.size<=best_tokn.size)
                        {
                            // too late !
                            src.unread(t);
                        }
                        else
                        {
                            // new winner
                            best_tokn.swap_with(t);
                            best_rule = r;
                            src.unread_a_copy_of(best_tokn);
                        }
                    }

                    //__________________________________________________________
                    //
                    //
                    // use best_rule
                    //
                    //__________________________________________________________
                    assert(src.read()>=best_tokn.size);

                    //__________________________________________________________
                    //
                    // modify source status
                    //__________________________________________________________
                    src.skip(best_tokn.size);


                    if(best_rule->ctrl)
                    {
                        //______________________________________________________
                        //
                        // this is a control rule
                        //______________________________________________________
                        ctrl = true;
                        if(best_rule->apply(best_tokn))
                        {
                            throw exception("<%s>: control rule '%s' must not produce a lexeme!", name.c_str(), best_rule->label.c_str());
                        }
                        return 0;
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // this is a regular rule
                        //______________________________________________________
                        ctrl = false;
                        if(best_rule->apply(best_tokn))
                        {
                            // produce a lexeme
                            return new lexeme(best_rule->label,this->line,best_tokn);
                        }
                        else
                        {
                            // drop...
                            continue;
                        }
                    }
                }
            }
            
            
        }
        
    }
    
}

