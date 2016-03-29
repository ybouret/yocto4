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

            static const char xnode_name [] = "XNODE";
            static const char walker_args[] = "const XNODE *node";


            static inline string label2method(const string &label)
            {
                string       ans = "";
                const size_t n = label.size();
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




            void optional:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! optional  : " << label << "\n";
            }

            void terminal:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! terminal  : " << label << "(" << xnode::get_property_text(modifier) << ")\n";
            }

            void alternate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! alternate : " << label << "\n";
            }

            void aggregate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! aggregate : " << label << "\n";

            }


            void at_least:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! at_least  : " << label << "\n";
            }


            namespace
            {
                class walker_rule
                {
                public:
                    const string   label;
                    const string   method;
                    const bool     internal;

                    inline walker_rule(const string &l) :
                    label(l),
                    method(label2method(label)),
                    internal( strchr( method.c_str(), grammar::internal_char ) )
                    {
                    }

                    inline  ~walker_rule() throw()
                    {}


                    const string &key() const throw() { return label; }

                    inline walker_rule(const walker_rule &other) :
                    label( other.label ),
                    method(other.method),
                    internal(other.internal)
                    {
                    }


                private:
                    YOCTO_DISABLE_ASSIGN(walker_rule);
                };

                typedef set<string,walker_rule> walker_db;


                static inline void register_walker_rule_in(walker_db    &wdb,
                                                           const rule   *r,
                                                           ios::ostream &nsfp)
                {
                    assert(r);
                    const walker_rule wr(r->label);
                    if( !wdb.insert(wr) )
                    {
                        throw exception("unexpected walker failure for '%s'", r->label.c_str());
                    }
                    ios::net_string::format(r->label,nsfp);
                }
            }



            void grammar:: walker_prolog(ios::ostream &fp,
                                         const string &class_name) const
            {
                walker_db       wdb(rules.size,as_capacity);

                //______________________________________________________________
                //
                // collect all seen rules
                //______________________________________________________________
                string        nsdb;       // net string databas
                ios::osstream nsfp(nsdb); // to write

                for(const rule *r = rules.head;r;r=r->next)
                {
                    bool do_register = true;
                    switch(r->uuid)
                    {
                        case terminal::UUID:
                        {
                            assert(r->content()!=NULL);
                            const property modifier = *static_cast<property *>(r->content());
                            if(jettison==modifier)
                            {
                                do_register = false;
                                std::cerr << "drop " << r->label << std::endl;
                            }
                        } break;

                        default:
                            break;
                    }
                    register_walker_rule_in(wdb,r,nsfp);
                }


                //______________________________________________________________
                //
                // dedicated walker
                //______________________________________________________________
                fp << "class " << class_name << "_base : public yocto::lang::syntax::walker {\n";
                fp << "public:\n";
                fp << "\tinline virtual ~" << class_name << "_base() throw() {}\n";
                fp << "\tinline explicit " << class_name << "_base() : yocto::lang::syntax::walker(\"" << nsdb << "\") {}\n";
                fp << "private:\n";
                fp << "\t" << class_name << "_base(const " << class_name << "_base &);\n";
                fp << "\t" << class_name << "_base &operator=(const " << class_name << "_base &);\n";
                fp << "};\n\n";

                //______________________________________________________________
                //
                // prolog
                //______________________________________________________________
                fp << "class " << class_name << " : public " << class_name << "_base {\n";
                fp << "public:\n";
                fp << "\ttypedef yocto::lang::syntax::xnode  " << xnode_name  << ";\n";

                fp << "\tvirtual ~" << class_name << "() throw();\n";
                fp << "\texplicit " << class_name << "() throw();\n";

                fp << "private:\n";
                //______________________________________________________________
                //
                // Emit know code and prototypes
                //______________________________________________________________
                for( const rule *r = rules.head;r;r=r->next)
                {
                    static const size_t ns = 64;
                    fp << "\t//";
                    for(size_t i=0;i<ns;++i) fp << "_";
                    fp << "\n";
                    fp << "\t//\n";

                    r->cpp(fp,class_name);
                    fp << "\t//";
                    for(size_t i=0;i<ns;++i) fp << "_";
                    fp << "\n";
                    fp << "\n";
                }
                
                fp << "public:\n";
                
            }
            
            
            void grammar:: walker_epilog(ios::ostream &fp, const string &class_name) const
            {
                //______________________________________________________________
                //
                // epilog
                //______________________________________________________________
                fp << "private:\n";
                fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << class_name << ");\n";
                fp << "};\n";
            }
            
        }
    }
}
