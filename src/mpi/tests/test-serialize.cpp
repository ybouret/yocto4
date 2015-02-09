#include "yocto/ios/serialize.hpp"
#include "yocto/mpi/mpi.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/memory/buffers.hpp"

using namespace yocto;

namespace
{
    class ParamsItems : public ios::serialize::items
    {
    public:
        ParamsItems() : ios::serialize::items(3)
        {
            add<double>("sigma");
            add<float>("epsilon");
            add<uint32_t>("uuid");
        }

        virtual ~ParamsItems() throw()
        {
        }

    private:

    };

    class Params : public ios::serialize::binary_data
    {
    public:
        double         &sigma;
        float          &epsilon;
        uint32_t       &uuid;

        explicit Params( const ios::serialize::items &srl) :
        ios::serialize::binary_data(srl),
        sigma(   as<double>  ("sigma")   ),
        epsilon( as<float>   ("epsilon") ),
        uuid(    as<uint32_t>("uuid")    )
        {


        }

        virtual ~Params() throw()
        {
        }

    private:

    };
}

YOCTO_UNIT_TEST_IMPL(serialize)
{
    YOCTO_MPI(MPI_THREAD_MULTIPLE);
    MPI.CloseStdIO();
    hashing::sha1 H;

    ParamsItems dataLayout;
    Params      data(dataLayout);

    if( MPI.IsFirst )
    {
        data.sigma   = 1.7;
        data.epsilon = -3.4f;
        data.uuid    = 90;
    }

    if(MPI.IsParallel)
    {
        memory::buffer_of<char,memory::global> iobuff( dataLayout.linear );
        if(MPI.IsFirst)
        {
            data.write(iobuff.rw());
        }
        MPI.Printf(stderr, "Hash=%08x\n", H.key<uint32_t>(iobuff) );
        MPI.Bcast(iobuff.rw(), iobuff.length(), MPI_BYTE, 0, MPI_COMM_WORLD);
        MPI.Printf0(stderr, "transmitted...\n");
        MPI.Printf(stderr, "Hash=%08x\n", H.key<uint32_t>(iobuff) );
        data.read(iobuff.ro());
        MPI.Printf(stderr, "sigma=%g, epsilon=%g, uuid=%u\n", data.sigma, data.epsilon, unsigned(data.uuid));
    }
    MPI.Printf0(stderr, "\n");


}
YOCTO_UNIT_TEST_DONE()
