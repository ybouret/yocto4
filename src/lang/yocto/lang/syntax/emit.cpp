#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/grammar.hpp"
#include "yocto/lang/syntax/walker.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include <iostream>

#include <cstring>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            static const char walker_name[] = "WALKER";
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


#if 0
            static inline void __check_terminal_is(const bool value, ios::ostream &fp)
            {
                fp << "\t\tassert(" << (value?"true":"false") << "==node->terminal);\n";
            }
#endif

            static inline bool __is_internal(const string &label) throw()
            {
                return 0 != strchr(label.c_str(),'#');
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


            namespace
            {
                class walker_rule                 {
                public:
                    const string   label;
                    const string   method;
                    const uint32_t hcode;

                    inline walker_rule(const string &l, hashing::function &H) :
                    label(l),
                    method(label2method(label)),
                    hcode( H.key<uint32_t>(label) )
                    {
                    }

                    inline  ~walker_rule() throw()
                    {}


                    const string &key() const throw() { return label; }

                    inline walker_rule(const walker_rule &other) :
                    label( other.label ),
                    method(other.method),
                    hcode( other.hcode )
                    {
                    }


                private:
                    YOCTO_DISABLE_ASSIGN(walker_rule);
                };

                typedef set<string,walker_rule> walker_db;

            }

            void grammar:: walker_prolog(ios::ostream &fp,
                                         const string &class_name) const
            {
                walker_db       wdb(rules.size,as_capacity);
                walker::hash_fn H;

                //______________________________________________________________
                //
                // collect all seen rules
                //______________________________________________________________
                for(const rule *r = rules.head;r;r=r->next)
                {
                    const walker_rule wr(r->label,H);
                    {
                        const uint32_t hcode = wr.hcode;
                        for( walker_db::iterator i=wdb.begin(); i!=wdb.end(); ++i)
                        {
                            if( hcode == (*i).hcode )
                            {
                                throw exception("Unexpected Multiple HashCode!");
                            }
                        }
                    }
                    if( !wdb.insert(wr) )
                    {
                        throw exception("unexpected walker failure for '%s'", r->label.c_str());
                    }
                    std::cerr << wr.label << " -> " << wr.method << " -> " << std::hex << wr.hcode << std::dec << std::endl;
                }




                //______________________________________________________________
                //
                // prolog
                //______________________________________________________________
                fp << "class " << class_name << " : public yocto::lang::syntax::walker {\n";
                fp << "public:\n";
                fp << "\ttypedef yocto::lang::syntax::xnode  " << xnode_name  << ";\n";
                fp << "\ttypedef yocto::lang::syntax::walker " << walker_name << ";\n";

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
