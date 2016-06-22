#include "yocto/associative/htree.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(htree)
{
    htree<double> H;
    vector<string> keys;
    for(size_t i=10+alea_lt(10);i>0;--i)
    {
        const string key = gen<string>::get();
        if( !H.insert(key,new double(i)) )
        {
            std::cerr << "Multiple key '" << key << "'" << std::endl;
        }
        keys.push_back(key);
    }

    std::cerr << "keys=" << keys << std::endl;
    for(size_t i=1;i<=keys.size();++i)
    {
        const double *pp = H.find(keys[i]);
        if(!pp) throw exception("cannot find '%s'", keys[i].c_str());
        std::cerr << keys[i] << " => " << *pp << std::endl;
    }

    std::cerr << "sizeof(htree::node_type)=" << sizeof(htree<double>::node_type) << std::endl;
    std::cerr << "#nodes=" << H.nodes() << std::endl;
    std::cerr << "#size =" << H.size() << "/" << keys.size() << std::endl;
}
YOCTO_UNIT_TEST_DONE()
