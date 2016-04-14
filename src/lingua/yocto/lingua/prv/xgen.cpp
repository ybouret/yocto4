#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            xgen:: ~xgen() throw()
            {
            }

            xgen:: xgen(const bool output_files) :
            lingua::parser("generator","main"),
            output(output_files)
            {

                //______________________________________________________________
                //
                //
                // grammar description grammar !
                //
                //______________________________________________________________
                Agg &GRAMMAR = agg("GRAMMAR");

                dict.define("ID", "[[:alpha:]_][:word:]*");

                Rule &END     = jettison(';');

                //______________________________________________________________
                //
                // fist item: the lang name...
                //______________________________________________________________
                {
                    Agg  &LANG_ID = agg("_LANG_ID",property::temporary);
                    LANG_ID << terminal("LANG_ID", "\\x2E{ID}") << END;
                    GRAMMAR << LANG_ID;
                }

                Agg &RULE    = agg("RULE");
                {
                    Rule &RULE_ID = terminal("RULE_ID","{ID}");
                    RULE << RULE_ID << jettison(':');

                }
                RULE << END;

                GRAMMAR << zero_or_more(RULE);
                
                //______________________________________________________________
                //
                // lexical rules
                //______________________________________________________________
                root.drop("blank", "[:blank:]");
                root.endl("endl");

                //______________________________________________________________
                //
                // done
                //______________________________________________________________
                if(output)
                {
                    graphviz("xgen.dot");
                    ios::graphviz_render("xgen.dot");
                }

            }

        }

        parser * parser:: generate(ios::istream &fp, const bool output_files)
        {
            syntax::xgen   generator(output_files);
            syntax::xnode *tree = generator.parse(fp);
            assert(tree);
            auto_ptr<syntax::xnode> guard(tree);
            if(output_files)
            {
                tree->graphviz("usr_tree.dot");
                ios::graphviz_render("usr_tree.dot");
            }
            return 0;
        }
        
    }
}
