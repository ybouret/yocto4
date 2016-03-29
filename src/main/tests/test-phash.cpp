#include "yocto/utest/run.hpp"
#include "yocto/core/list.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/comparator.hpp"

#include "yocto/hashing/perfect.hpp"
#include "yocto/ios/graphviz.hpp"
#include <cstdlib>

using namespace yocto;



YOCTO_UNIT_TEST_IMPL(phash)
{
    std::cerr << "sizeof(hashing::perfect::node_type)=" << sizeof(hashing::perfect::node_type) << std::endl;
    vector<string> words;
    {
        string line;
        ios::icstream fp( ios::cstdin );
        while( line.clear(), fp.read_line(line) >= 0 ) words.push_back(line);
    }
    std::cerr << "words=" << words  << std::endl;
    string s = "dummy";

    hashing::perfect mph;
    for(size_t i=1;i<=words.size();++i)
    {
        mph.insert(words[i],i-1);
    }
    mph.optimize();
    std::cerr << std::endl;
    std::cerr << "\t\t #mph nodes=" << mph.nodes << std::endl;
    std::cerr << "\t\t sizeof(mph node)=" << sizeof(hashing::perfect::node_type) << std::endl;
    for(size_t i=1;i<=words.size();++i)
    {
        std::cerr << "mph(" << words[i] <<")=" << mph(words[i]) << std::endl;
    }
    std::cerr << "mph(" << s << ")=" << mph(s) << std::endl;

    mph.graphviz("htree.dot");
    ios::graphviz_render("htree.dot");
    //(void) system("dot -Tpng -o htree.png htree.dot");
    
    const char *other[] =
    {
        "for",
        "while",
        "if",
        "then"
    };
    
    const hashing::perfect kw(YOCTO_PERFECT_HASHER_FOR(other));
    
}
YOCTO_UNIT_TEST_DONE()
