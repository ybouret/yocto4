#include "yocto/utest/run.hpp"
#include "yocto/net/io/queue.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace network;


YOCTO_UNIT_TEST_IMPL(io)
{
    size_t   block_size = 8;
    if(argc>1)
    {
        block_size = strconv::to<size_t>(argv[1],"block_size");
    }
    io_cache cache(block_size);
    io_queue Q(cache);


    char buffer[8];

    size_t nch = 0;
    {
        ios::icstream fp( ios::cstdin );
        string line;
        while(line.clear(), fp.read_line(line) > 0 )
        {
            nch += line.size();
            size_t done = 0;
            Q.put_all(line,done);
            const size_t nb = 1+ alea_lt(sizeof(buffer));
            Q.get(buffer,nb,done);
        }
    }

    std::cerr << std::endl;
    std::cerr << "nch    = " << nch << std::endl;
    std::cerr << "Q.bytes= " << Q.bytes() << std::endl;
    std::cerr << "Q.count= " << Q.count() << " block(s)" << std::endl;

    std::cerr << "C.bytes= " << cache.bytes() << std::endl;
    std::cerr << "C.count= " << cache.bytes() << std::endl;

    std::cerr << std::endl << "...free..." << std::endl;
    Q.free();
    std::cerr << "Q.bytes= " << Q.bytes() << std::endl;
    std::cerr << "Q.count= " << Q.count() << " block(s)" << std::endl;

    std::cerr << "C.bytes= " << cache.bytes() << std::endl;
    std::cerr << "C.count= " << cache.bytes() << std::endl;


}
YOCTO_UNIT_TEST_DONE()