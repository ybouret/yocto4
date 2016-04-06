#include "yocto/lingua/regexp/compiler.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/lingua/pattern/posix.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/imstream.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(regexp)
{
    p_dict dict;
    dict.define("DIGIT","[0-9]");
    dict.define("INT","{DIGIT}+");


    if(argc>1)
    {
        const string      expr = argv[1];
        auto_ptr<pattern> p( regexp::compile(expr,&dict) );
        p->graphviz("rx.dot");
        ios::graphviz_render("rx.dot");

        {
            ios::wcstream fp("rx.bin");
            p->save(fp);
        }

        ios::icstream fp( ios::cstdin );
        string line;
        while( (std::cerr << "> ").flush(), line.clear(), fp.gets(line) )
        {
            ios::imstream input(line);
            source        src(input);
            while(src.is_active())
            {
                token         tkn;
                if(p->match(tkn,src))
                {
                    std::cerr << "<" << tkn << ">";
                }
                else
                {
                    assert(0==tkn.size);
                    auto_ptr<t_char> tmp( src.query() );
                    assert(tmp.is_valid());
                    std::cerr << char(tmp->code);
                }
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;

    }


}
YOCTO_UNIT_TEST_DONE()
