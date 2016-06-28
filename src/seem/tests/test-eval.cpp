#include "yocto/utest/run.hpp"
#include "yocto/seem/evaluator.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/graphviz.hpp"
#include "yocto/hashing/sha1.hpp"
#include <cmath>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(eval)
{
    hashing::sha1   H;
    Seem::Evaluator::Pointer SEEM( new Seem::Evaluator );
    SEEM->SetVariable("x", 0.1);

    for(int i=1;i<argc;++i)
    {
        const char *expr = argv[i];
        std::cerr << expr << "=" << SEEM->run(expr) << std::endl;
        const Seem::vCode vcode = SEEM->compile(expr);
        std::cerr << expr << "=" << SEEM->eval(vcode) << std::endl;
        const uint32_t key = H.key<uint32_t>(expr,length_of(expr));
        const string   fname =  vformat("code_x%08x.bin",key);
        {
            std::cerr << "saving to " << fname << std::endl;
            ios::wcstream  fp(fname);
            vcode->save(fp);
        }

        {
            std::cerr << "re-loading..." << std::endl;
            ios::icstream fp(fname);
            const Seem::vCode new_vcode = SEEM->load(fp);
            std::cerr << fname << "=" << SEEM->eval(new_vcode) << std::endl;
        }

    }


}
YOCTO_UNIT_TEST_DONE()

