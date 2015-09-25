#include "yocto/R/IR.hpp"
#include "yocto/program.hpp"
#include "yocto/mpa/rational.hpp"

using namespace yocto;


YOCTO_PROGRAM_START()
{
    CMatrix<int> iM(3,4);
    std::cerr << "iM=" << iM << std::endl;

    
    CMatrix<mpn> nM(4,5);
    std::cerr << "nM=" << nM << std::endl;

    CMatrix<mpq> qM(5,6);
    std::cerr << "qM=" << nM << std::endl;

}
YOCTO_PROGRAM_END()
