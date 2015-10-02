#include "yocto/R/IR.hpp"
#include "yocto/program.hpp"
#include "yocto/mpa/rational.hpp"

using namespace yocto;


YOCTO_PROGRAM_START()
{
    CMatrix<int> iM(3,4);
    iM.ld(1);
    std::cerr << "iM=" << iM << std::endl;


    
    CMatrix<mpn> nM(4,5);
    const mpn vn(1);
    nM.ld(vn);
    std::cerr << "nM=" << nM << std::endl;

    CMatrix<mpq> qM(5,6);
    const mpq half(1,2);
    qM.ld(half);
    std::cerr << "qM=" << qM << std::endl;


    MatricesOf<int,CMatrix> MM(4,5,6);
    
}
YOCTO_PROGRAM_END()
