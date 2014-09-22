#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(32)
{
    YOCTO_UNIT_TEST_DECL(mem);
    YOCTO_UNIT_TEST_DECL(io);
    YOCTO_UNIT_TEST_DECL(add);
    YOCTO_UNIT_TEST_DECL(add_perf);
    YOCTO_UNIT_TEST_DECL(cmp);
    YOCTO_UNIT_TEST_DECL(sub);
    YOCTO_UNIT_TEST_DECL(mul);
    YOCTO_UNIT_TEST_DECL(bits);
    YOCTO_UNIT_TEST_DECL(div);
    YOCTO_UNIT_TEST_DECL(ari);
    YOCTO_UNIT_TEST_DECL(parse);
    YOCTO_UNIT_TEST_DECL(prime);
    YOCTO_UNIT_TEST_DECL(rsa);
    YOCTO_UNIT_TEST_DECL(w2m);
    YOCTO_UNIT_TEST_DECL(rsa_keys);
    YOCTO_UNIT_TEST_DECL(rsa_perf);
    YOCTO_UNIT_TEST_DECL(mul_perf);
    YOCTO_UNIT_TEST_DECL(primality);
    YOCTO_UNIT_TEST_DECL(rsa_auth);

    YOCTO_UNIT_TEST_DECL(int);
    YOCTO_UNIT_TEST_DECL(pow);

    YOCTO_UNIT_TEST_DECL(rational);
    YOCTO_UNIT_TEST_DECL(split);

}
YOCTO_UNIT_TEST_EXEC()
