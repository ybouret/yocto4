#include "yocto/lingua/parser.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/lingua/syntax/analyzer.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(gen)
{
    auto_ptr<parser> P;
    if(argc>1)
    {
        {
            const string  filename = argv[1];
            ios::icstream fp(filename);
            P.reset( parser::generate(fp,true) );
            std::cerr << "-- ready" << std::endl;
        }

        

        {
            ios::icstream fp( ios::cstdin );
            auto_ptr<syntax::xnode> tree( P->parse(fp) );
            const string outname =P->grammar::name + "_output.dot";
            tree->graphviz(outname);
            ios::graphviz_render(outname);

            const string binfile =P->grammar::name + "_output.bin";
            {
                ios::wcstream bin(binfile);
                tree->save(bin);
            }

            {
                ios::icstream bin(binfile);
                auto_ptr<syntax::xnode> backup( syntax::xnode::load(bin, *P) );
                const string backname =P->grammar::name + "_backup.dot";
                backup->graphviz(backname);
                ios::graphviz_render(backname);

            }

            syntax::analyzer walker(*P);
            std::cerr << "-- WALKING..." << std::endl;
            ios::ocstream output( ios::cstderr );
            walker.walk(tree.__get(), &output);
        }
    }



}
YOCTO_UNIT_TEST_DONE()

