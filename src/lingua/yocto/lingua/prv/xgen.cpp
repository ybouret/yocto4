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
                // grammar description grammar !
                //______________________________________________________________
                dict.define("ID", "[[:alpha:]_][:word:]*");
                (**dict.search("ID")).graphviz("id.dot");
                ios::graphviz_render("id.dot");
                
                Rule &END     = jettison(';');

                Agg  &LANG_ID = agg("_LANG_ID",property::temporary);
                LANG_ID << terminal("LANG_ID", "\\x2E{ID}") << END;


                Agg &GRAMMAR = agg("GRAMMAR");
                GRAMMAR << LANG_ID;

                top_level(GRAMMAR);
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
