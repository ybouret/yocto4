#include "yocto/lingua/pattern/regexp.hpp"
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
    dict.define("ID", "[[:alpha:]_][:word:]*");


    if(argc>1)
    {
        const string      expr = argv[1];
        auto_ptr<pattern> p( regexp(expr,&dict) );
        p->graphviz("rx.dot");
        ios::graphviz_render("rx.dot");

        {
            std::cerr << "\tsaving..." << std::endl;
            ios::wcstream fp("rx.bin");
            p->save(fp);
        }

        {
            ios::icstream fp("rx.bin");
            auto_ptr<pattern> q( pattern::load(fp) );
            std::cerr << "\tloading..." << std::endl;
            const string p64 = p->b64();
            const string q64 = q->b64();
            std::cerr << "\t" << p64 << std::endl;
            std::cerr << "\t" << q64 << std::endl;
        }

        ios::icstream fp( ios::cstdin );
        string        line;
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
