#include "yocto/associative/htree.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "support.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;

namespace
{
    static inline void print_double(const double &x, ios::ostream &fp )
    {
        fp("<%g>",x);
    }
}

YOCTO_UNIT_TEST_IMPL(htree)
{
    htree<double>  H;
    vector<string> keys;

#if 0
    int i=0;
    {
        ios::icstream fp( ios::cstdin );
        string key;
        while( fp.gets(key) )
        {
            if( !H.insert(key,++i) )
            {
                std::cerr << "Multiple key '" << key << "'" << std::endl;
            }
            keys.push_back(key);
        }
    }
#else
    for(size_t i=10+alea_lt(10);i>0;--i)
    {
        const string key = gen<string>::get();
        if( !H.insert(key,i) )
        {
            std::cerr << "Multiple key '" << key << "'" << std::endl;
        }
        keys.push_back(key);
    }
#endif

    H.graphviz("htree_raw.dot", print_double);
    ios::graphviz_render("htree_raw.dot");

    H.optimize();
    H.graphviz("htree_opt.dot", print_double);
    ios::graphviz_render("htree_opt.dot");


    std::cerr << "keys=" << keys << std::endl;
    for(size_t i=1;i<=keys.size();++i)
    {
        const double *pp = H.find(keys[i]);
        if(!pp) throw exception("cannot find '%s'", keys[i].c_str());
        std::cerr << keys[i] << " => " << *pp << std::endl;
    }

    std::cerr << "sizeof(htree<double>::node_type)=" << sizeof(htree<double>::node_type) << std::endl;
    std::cerr << "sizeof(htree<string>::node_type)=" << sizeof(htree<string>::node_type) << std::endl;

    std::cerr << "#nodes=" << H.nodes() << std::endl;
    std::cerr << "#size =" << H.size() << "/" << keys.size() << std::endl;

    std::cerr << "#bytes=" << H.bytes() << std::endl;

}
YOCTO_UNIT_TEST_DONE()
