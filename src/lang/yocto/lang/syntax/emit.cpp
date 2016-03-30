#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/grammar.hpp"
#include "yocto/lang/syntax/walker.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/ios/net-string.hpp"

#include <iostream>

#include <cstring>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

#if 0
            static const char xnode_name  [] = "XNODE";
            static const char leaves_name [] = "LEAVES";
            static const char walker_args [] = "const XNODE *node";



            namespace
            {
                static inline string label2method(const string &label)
                {
                    string       ans = "on_";
                    const size_t n   = label.size();
                    for(size_t i=0;i<n;++i)
                    {
                        const char C = label[i];
                        if( isalnum(C) || '_' == C)
                        {
                            ans += C;
                        }
                        else
                        {
                            const uint8_t B(C);
                            ans += '_';
                            ans += hexa_text[B];
                        }
                    }
                    return ans;
                }

                static inline bool __is_internal(const string &label) throw()
                {
                    return (0 != strchr( label.c_str(), grammar::internal_char ) );
                }

                class walker_rule
                {
                public:
                    const string   label;
                    const string   method;

                    inline walker_rule(const string &l) :
                    label(l),
                    method(label2method(label))                    {
                    }

                    inline  ~walker_rule() throw()
                    {}


                    const string &key() const throw() { return label; }

                    inline walker_rule(const walker_rule &other) :
                    label( other.label ),
                    method(other.method)
                    {
                    }


                private:
                    YOCTO_DISABLE_ASSIGN(walker_rule);
                };

                typedef set<string,walker_rule> _walker_db;

                class walker_db : public _walker_db
                {
                public:
                    hashing::mperf mph;

                    explicit walker_db(const size_t n) : _walker_db(n,as_capacity) {}
                    virtual ~walker_db() throw() {}

                    inline void add( const rule *r, ios::ostream &nsfp)
                    {
                        assert(r);
                        //-- make local perfect hash
                        mph.insert(r->label,size());


                        //-- local database of method
                        const walker_rule wr(r->label);
                        if( !insert(wr) )
                        {
                            throw exception("unexpected walker failure for '%s'", r->label.c_str());
                        }

                        //-- store info for binary code
                        ios::net_string::format(wr.label,nsfp);

                    }

                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(walker_db);
                };


                static inline void __assert_terminal_is(const bool flag, ios::ostream &fp)
                {
                    fp << "\t\tassert(" << (flag?"true":"false") << "==node->terminal);\n";
                }
            }
#endif


            namespace
            {

                //! walker rule
                class wrule
                {
                public:
                    const rule    *pRule;    //!< rule address
                    const string   cname;    //!< C++ name
                    const bool     internal; //!< is internal ?

                    const string & key() const throw() { return pRule->label; }

                    inline wrule(const rule *r) :
                    pRule(r),
                    cname( label2cname(pRule->label) ),
                    internal( strchr( pRule->label.c_str(), grammar::internal_char) != NULL )
                    {
                    }

                    inline wrule(const wrule &wr) :
                    pRule(wr.pRule),
                    cname(wr.cname),
                    internal(wr.internal)
                    {
                    }

                    inline ~wrule() throw()
                    {
                    }



                private:
                    YOCTO_DISABLE_ASSIGN(wrule);
                    static string label2cname(const string &s)
                    {
                        string ans;
                        const size_t n   = s.size();
                        for(size_t i=0;i<n;++i)
                        {
                            const char C = s[i];
                            if( isalnum(C) || '_' == C)
                            {
                                ans += C;
                            }
                            else
                            {
                                const uint8_t B(C);
                                ans += '_';
                                ans += hexa_text[B];
                            }
                        }

                        return ans;
                    }
                };


                typedef set<string,wrule> _wmaker;

                class wmaker : public _wmaker
                {
                public:
                    string         nsdb; //!< netstring database
                    ios::osstream  nsfp; //!< wrapper to write into nsdb
                    hashing::mperf mph;
                    const string   class_name;
                    const string   class_name_base;

                    explicit wmaker(const size_t n,const string &user_class_name) :
                    _wmaker(n,as_capacity),
                    nsdb(),
                    nsfp(nsdb),
                    mph(),
                    class_name(user_class_name),
                    class_name_base(class_name+"_base")
                    {
                    }

                    virtual ~wmaker() throw()
                    {
                    }


                    inline void add( const rule *r )
                    {
                        bool do_register = true;
                        switch(r->uuid)
                        {
                            case terminal::UUID: {
                                assert(r->content());
                                const property modifier = *(property*)(r->content());
                                if(jettison==modifier)
                                {
                                    do_register = false;
                                }
                            } break;

                            default:
                                break;
                        }

                        if(do_register)
                        {
                            // register for class_name hashing
                            ios::net_string::format(r->label, nsfp);

                            // register for local hashing
                            mph.insert(r->label, size());

                            // register for class
                            const wrule wr(r);
                            if(!insert(wr))
                            {
                                throw exception("unexpected multiple rule label '%s'", r->label.c_str());
                            }
                        }

                    }

                    inline void export_base( ios::ostream &fp ) const
                    {

                    }


                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(wmaker);
                };


            }




            void optional:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
            }


            void terminal:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
            }

            void alternate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
            }

            void aggregate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
            }


            void at_least:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
            }




            void grammar:: walker_prolog(ios::ostream &fp,
                                         const string &class_name) const
            {

                wmaker wmk( rules.size, class_name);
                for(const rule *r = rules.head;r;r=r->next)
                {
                    wmk.add(r);
                }
            }

            
            void grammar:: walker_epilog(ios::ostream &fp,
                                         const string &class_name) const
            {

            }
            
        }
    }
}
