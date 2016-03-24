#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/grammar.hpp"

#include "yocto/code/utils.hpp"

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

            void optional:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// optional: " << label << "\n";
                const string method = label2method(label);
                fp << "\tvoid " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                fp << "\t\tassert(false==node->terminal);\n";
                fp << "\t}\n";
            }

            void terminal:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// terminal: " << label;
                if(standard==modifier)
                {
                    fp << "\n";
                }
                else
                {
                    fp << " (-)\n";
                }
            }

            void alternate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// alternate: " << label << "\n";
                const string method = label2method(label);
                fp << "\tvoid " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                fp << "\t}\n";
            }

            void aggregate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// aggregate: " << label << "\n";
                const string method = label2method(label);
                fp << "\tvoid " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                fp << "\t}\n";
            }


            void at_least:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t// optional: " << label << "\n";
                const string method = label2method(label);
                fp << "\tvoid " << method << "(" << walker_args << ")\n";
                fp << "\t{\n";
                fp << "\t}\n";
            }

            void grammar:: walker_prolog(ios::ostream &fp,
                                         const string &class_name) const
            {
                
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
