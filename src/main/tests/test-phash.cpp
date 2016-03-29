#include "yocto/utest/run.hpp"
#include "yocto/core/list.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/comparator.hpp"

#include "yocto/ios/graphviz.hpp"
#include <cstdlib>

using namespace yocto;

#include "yocto/hashing/mph.hpp"

YOCTO_UNIT_TEST_IMPL(mph)
{
    //std::cerr << "sizeof(core_list)=" << sizeof(core::list_of<hashing::mp_hasher::node_type>)     << std::endl;
    //std::cerr << "sizeof(_cpp_list)=" << sizeof(core::list_of_cpp<hashing::mp_hasher::node_type>) << std::endl;
    std::cerr << "sizeof(mperf::node_type)=" << sizeof(hashing::mperf::node_type) << std::endl;

    vector<string> words;
    {
        string line;
        ios::icstream fp( ios::cstdin );
        while( line.clear(), fp.read_line(line) >= 0 ) words.push_back(line);
    }
    std::cerr << "words=" << words  << std::endl;
    string s = "dummy";

    hashing::mperf mph;
    for(size_t i=1;i<=words.size();++i)
    {
        std::cerr << "-- insert '" << words[i] << "'" << std::endl;
        mph.insert(words[i],i-1);
    }
    std::cerr << std::endl;
    std::cerr << "\t\t #mph nodes=" << mph.nodes << std::endl;
    mph.graphviz("mph_raw.dot");
    ios::graphviz_render("mph_raw.dot");

    mph.optimize();
    mph.graphviz("mph_opt.dot");
    ios::graphviz_render("mph_opt.dot");

    for(size_t i=1;i<=words.size();++i)
    {
        std::cerr << "mph(" << words[i] <<")=" << mph(words[i]) << std::endl;
    }
    std::cerr << "mph(" << s << ")=" << mph(s) << std::endl;

    std::cerr << "echo \x32" << std::endl;

    const char *other[] =
    {
        "for",
        "while",
        "if",
        "then"
    };

    const hashing::mperf kw(YOCTO_MPERF_FOR(other));
    kw.graphviz("kw.dot");
    ios::graphviz_render("kw.dot");

}
YOCTO_UNIT_TEST_DONE()
