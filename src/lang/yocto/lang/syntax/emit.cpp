#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/grammar.hpp"

#include "yocto/code/utils.hpp"

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


            static inline void __check_terminal_is(const bool value, ios::ostream &fp)
            {
                fp << "\t\tassert(" << (value?"true":"false") << "==node->terminal);\n";
            }

            static inline bool __is_internal(const string &label) throw()
            {
                return 0 != strchr(label.c_str(),'#');
            }

            void optional:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// optional: '" << label << "'\n";
                const string method = label2method(label);
                fp << "\tinline void " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                __check_terminal_is(false,fp);
                fp << "\t}\n";
            }

            void terminal:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// terminal: '" << label << "' (" << xnode::get_property_text(modifier) << ")\n";
                if(jettison==modifier)
                {
                    return;
                }
                else
                {
                    const string method = label2method(label);
                    fp << "void " << method << "(" << walker_args << "); // MUST BE IMPLEMENTED!\n";
                }
            }

            void alternate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// alternate: '" << label << "'\n";
                const string method = label2method(label);
                fp << "\tinline void " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                __check_terminal_is(false,fp);
                fp << "\t}\n";
            }

            void aggregate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// aggregate: '" << label << "'(" << xnode::get_property_text(modifier) << ")\n";
                const string method = label2method(label);
                fp << "\tvoid " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                __check_terminal_is(false,fp);
                fp << "\t}\n";
            }


            void at_least:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// at_least: '" << label << "'\n";
                const string method = label2method(label);
                fp << "\tinline void " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                __check_terminal_is(false,fp);
                fp << "\t}\n";
            }


            namespace
            {
                class walker_rule : public object
                {
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

                    inline virtual ~walker_rule() throw()
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
                walker_db db(rules.size,as_capacity);

                //______________________________________________________________
                //
                // collect all seen rules: exclude jettison...
                //______________________________________________________________


                //______________________________________________________________
                //
                // prolog
                //______________________________________________________________
                fp << "class " << class_name << " : public yocto::lang::syntax::walker {\n";
                fp << "public:\n";
                fp << "\ttypedef yocto::lang::syntax::xnode  " << xnode_name << ";\n";
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
