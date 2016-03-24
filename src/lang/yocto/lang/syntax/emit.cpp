#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/grammar.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            static const char walker_name[] = "yocto::lang::syntax::walker";
            static const char xnode_name[]  = "XNODE";

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


            void grammar:: walker_proto(ios::ostream &fp,
                                        const string &class_name) const
            {
                //______________________________________________________________
                //
                // prolog
                //______________________________________________________________
                fp << "class " << class_name << " : public " << walker_name << "{\n";
                fp << "public:\n";
                fp << "typedef yocto::lang::syntax::xnode " << xnode_name << ";\n";
                fp << "\tinline virtual ~" << class_name << "() throw() {}\n";

                //______________________________________________________________
                //
                // epilog
                //______________________________________________________________
                fp << "private:\n";
                fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(class_name);\n";
                fp << "};\n";
            }

        }
    }
}
