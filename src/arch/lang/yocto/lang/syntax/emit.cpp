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
                    internal( strchr( pRule->label.c_str(), rule::internal_char) != NULL )
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
                        string ans = "on_";
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

                static const char walker_args[] = "const XNODE *node";
                
                class wmaker : public _wmaker
                {
                public:
                    ios::ostream  &fp;   //!< the output
                    string         nsdb; //!< netstring database
                    ios::osstream  nsfp; //!< wrapper to write into nsdb
                    hashing::mperf mph;
                    const string   class_name;
                    const string   class_name_base;

                    explicit wmaker(const size_t n,const string &user_class_name, ios::ostream &user_fp) :
                    _wmaker(n,as_capacity),
                    fp(user_fp),
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

                    //__________________________________________________________
                    //
                    // prepare databases of rules
                    //__________________________________________________________
                    inline void add_one( const rule *r )
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
                            nsfp.flush();

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

                    inline void add( const core::list_of<rule> &rr )
                    {
                        for(const rule *r = rr.head;r;r=r->next)
                        {
                            add_one(r);
                        }
                    }

                    //__________________________________________________________
                    //
                    // export base class
                    //__________________________________________________________
                    inline void export_base() const
                    {
                        fp << "class " << class_name_base << " : public yocto::lang::syntax::walker {\n";
                        fp << "public:\n";

                        fp << "\tinline explicit " << class_name_base << "() : yocto::lang::syntax::walker(\"" << nsdb << "\")\n\t{}\n";
                        fp << "\tinline virtual ~" << class_name_base << "() throw() {}\n";
                        fp << "private:\n";
                        fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << class_name_base << ");\n";
                        fp << "};\n\n";
                    }

                    //__________________________________________________________
                    //
                    // start a comment
                    //__________________________________________________________
                    inline ios::ostream & com() const
                    {
                        fp << "\t\t//";
                        return fp;
                    }

                    //__________________________________________________________
                    //
                    // make a horizontal line
                    //__________________________________________________________
                    inline void hline() const
                    {
                        com(); for(size_t i=0;i<64;++i) fp << '_'; fp << '\n';
                    }

                    //__________________________________________________________
                    //
                    // start class prolog
                    //__________________________________________________________
                    inline void export_prolog( ) const
                    {
                        fp << "class " << class_name << " : public " << class_name_base << " {\n";
                        fp << "public:\n";
                        fp << "\ttypedef yocto::lang::syntax::xnode XNODE;\n";
                        fp << "\texplicit " << class_name << "();          //!< implemented by user\n";
                        fp << "\tvirtual ~" << class_name << "() throw() ; //!< implemented by user\n";
                    }

                    //__________________________________________________________
                    //
                    // export all rules
                    //__________________________________________________________
#define YWK_LABEL(NAME) case NAME ::UUID: fp << " " #NAME " : '" << r->label << "'\n"; export_##NAME(w); break

                    inline void export_rules() const
                    {
                        fp << "private:\n";
                        for(const_iterator i=begin();i!=end();++i)
                        {
                            const wrule &w  = *i;
                            const rule  *r  = w.pRule;
                            hline();
                            com() << '\n';
                            com();
                            switch(r->uuid)
                            {
                                    YWK_LABEL(terminal);
                                    YWK_LABEL(optional);
                                    YWK_LABEL(at_least);
                                    YWK_LABEL(alternate);
                                    YWK_LABEL(aggregate);

                                default:
                                    throw exception("unrecognized rule uuid!");
                            }
                            hline();
                            
                            fp << '\n';
                        }
                    }

                    
                    inline void prepare_public_api() const
                    {
                        fp << "public:\n";
                        hline();
                        com() << "\n";
                        com() << "user's data\n";
                        hline();
                    }
                    
                    inline ios::ostream & proto(const wrule &w) const
                    {
                        fp << "\t\tvoid " <<  w.cname << '(' << walker_args << ')';
                        return fp;
                    }
                    
                    //__________________________________________________________
                    //
                    // export terminal
                    //__________________________________________________________
                    inline void export_terminal(const wrule &w) const
                    {
                        proto(w) << "; // to be implemented\n";
                    }

                    //__________________________________________________________
                    //
                    // export optional
                    //__________________________________________________________
                    inline void export_optional(const wrule &w) const
                    {
                        proto(w) << ";\n";
                    }
                    
                    //__________________________________________________________
                    //
                    // export at_least
                    //__________________________________________________________
                    inline void export_at_least(const wrule &w) const
                    {
                        proto(w) << ";\n";

                    }
                    
                    //__________________________________________________________
                    //
                    // export alternate
                    //__________________________________________________________
                    inline void export_alternate(const wrule &w) const
                    {
                        proto(w) << ";\n";

                    }
                    
                    //__________________________________________________________
                    //
                    // export aggregate
                    //__________________________________________________________
                    inline void export_aggregate(const wrule &w) const
                    {
                        proto(w) << ";\n";

                    }
                    
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(wmaker);
                };


            }



            void grammar:: walker_prolog(ios::ostream &fp,
                                         const string &class_name) const
            {
                
                wmaker wmk( rules.size, class_name, fp);
                wmk.add(rules);
                wmk.export_base();
                wmk.export_prolog();
                wmk.export_rules();
                wmk.prepare_public_api();
                
            }
            
            
            void grammar:: walker_epilog(ios::ostream &fp,
                                         const string &class_name) const
            {
                //______________________________________________________________
                //
                // epilog
                //______________________________________________________________
                fp << "private:\n";
                fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << class_name << ");\n";
                fp << "};\n\n";
            }
            
        }
    }
}
